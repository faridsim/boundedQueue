# ADR-0001: Use Mutex-Based Synchronization

## Status

Accepted

## Date

2026-07-29

## Context

Boundflow is a bounded concurrent queue intended for producer-consumer
workloads.

When the queue is empty, consumers may need to wait. When it is full,
producers may need to wait. In the expected workload, threads often have
no useful alternative work while their queue is unavailable.

The synchronization mechanism should therefore:

- protect shared queue state;
- support sleeping waits for full and empty conditions;
- avoid consuming CPU while no progress is possible;
- remain simple enough to review and test;
- support waking blocked operations when the queue closes.

Queue micro-optimizations are not currently the main performance concern.
Workers are expected to spend substantially more time processing items
than interacting with the queue.

## Decision Drivers

- avoid busy waiting during potentially meaningful delays;
- directly support blocking producer and consumer operations;
- keep synchronization state small and understandable;
- use standard-library facilities;
- avoid lock-free complexity without evidence that the queue is a
  bottleneck;
- support explicit queue closure.

## Considered Options

### Lock-free queue with waiting outside the queue

This approach keeps the queue focused on immediate operations and moves
sleeping and wake-up decisions into a scheduler or another external
component.

It is useful when one thread must coordinate several queues or events.

For a simple pipeline in which a thread communicates with one queue,
however, external waiting adds machinery without giving the thread
another useful action to perform.

### Blocking through busy waiting

A thread repeatedly checks whether the queue has become usable.

This may be appropriate when waits are very uncommon and expected to
finish almost immediately.

It is not appropriate for the expected workload because a stalled
producer or consumer may leave another thread consuming CPU for a
meaningful period. Waiting producers may also compete for CPU time with
consumers that are performing useful work.

### Heavier synchronization design

A queue can combine several mutexes, condition variables, counters, and
semaphore-like mechanisms.

Such a design can support blocking, but it introduces more
synchronization state than the current queue requires.

### One mutex and two condition variables

One mutex protects queue contents and lifecycle state.

One condition variable represents the possibility that an element has
become available. The other represents the possibility that capacity
has become available.

This directly supports sleeping waits with a small synchronization
model.

## Decision

Use one mutex and two condition variables.

- `not_empty` is used by consumers waiting for an element.
- `not_full` is used by producers waiting for capacity.
- immediate operations use the same mutex but do not wait for occupancy
  to change.
- closing the queue wakes all producers and consumers so they can
  observe the closed state.

Lock-free synchronization is not adopted at this stage.

The project will first use the simpler blocking design and reconsider it
only if measurement shows that queue synchronization is a meaningful
bottleneck.

## Consequences

### Positive

- waiting threads can sleep rather than consume CPU continuously;
- full and empty conditions are represented explicitly;
- one mutex protects related queue state transitions;
- blocking and immediate operations share the same synchronization
  boundary;
- closure can release operations waiting on either condition;
- the implementation remains small and understandable.

### Negative

- operations are not lock-free or wait-free;
- concurrent operations may contend for the mutex;
- this design may not suit an extreme low-latency workload.

### Accepted trade-off

The project accepts possible mutex contention in exchange for sleeping
waits, simpler reasoning, and lower implementation complexity.

## Validation

This decision is reflected by:

- one queue mutex;
- separate `not_empty` and `not_full` condition variables;
- predicate-based waiting in blocking operations;
- notification after insertion and removal;
- notification of all waiters during closure.

Tests should verify blocking, wake-up, capacity enforcement, and closure
behaviour.

## Reconsider When

Reconsider this decision when:

- profiling identifies the queue mutex as a significant bottleneck;
- the project gains an explicit lock-free progress requirement;
- blocking is consistently rare and extremely short;
- waiting is moved into a higher-level scheduler;
- the workload requires substantially lower queue-operation latency.

A replacement should be recorded in a superseding ADR.

## Related Decisions

- ADR-0002: Use `std::deque` as the initial backing store
- ADR-0003: Keep queue identity stable
- ADR-0004: Expose blocking and immediate operations