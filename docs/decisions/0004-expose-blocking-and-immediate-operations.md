# ADR-0004: Expose Blocking and Immediate Operations

## Status

Accepted

## Date

2026-07-29

## Context

A bounded queue cannot always complete an operation immediately:

- insertion cannot proceed while the queue is full;
- removal cannot proceed while the queue is empty.

Different callers need different behaviour.

In a simple producer-consumer pipeline, a thread may interact with one
queue and have no useful alternative work while the queue is
unavailable. Blocking inside the queue is appropriate in that context.

In a more complex system, a thread may need to coordinate several
queues, events, or possible actions. Blocking indefinitely inside one
queue would prevent that higher-level coordination.

The queue should therefore provide synchronization mechanics without
forcing one waiting policy on every caller.

## Decision Drivers

- support convenient sleeping waits for simple queue usage;
- allow callers to retain control when waiting is inappropriate;
- make the choice visible through the selected operation;
- keep occupancy checks and queue transitions synchronized;
- avoid requiring callers to implement polling for ordinary blocking
  use;
- leave multi-queue coordination to higher-level components;
- remain consistent with the current lifecycle and result model.

## Considered Options

### Blocking operations only

This would keep simple producer-consumer usage straightforward.

It would not support callers that need to inspect several queues or
choose another action when the current queue is unavailable.

### Immediate operations only

This would always return control to the caller.

Simple callers would need to create their own waiting mechanism.
Repeated attempts without a sleeping mechanism could also become busy
waiting.

### Separate blocking and immediate operations

Provide one operation for callers willing to wait and another for
callers that need an immediate result.

This allows the queue to support both simple and higher-level usage
without embedding scheduling policy.

## Decision

Expose separate blocking and immediate operations.

The current interface is:

```cpp
op::status push(T&& item);
bool try_push(T&& item);
op::status pop(T& item);
bool try_pop(T& item);
```

The blocking operations use `op::status` because closure can terminate
the operation:

- `op::status::SUCCEED` means the queue operation completed;
- `op::status::closed` means closure prevented completion or the closed
  queue had no remaining element to remove.

The immediate operations currently use `bool`:

- `true` means the operation completed;
- `false` means the required occupancy condition was not available
  immediately.

The caller chooses waiting behaviour by choosing the operation.

## Responsibility Boundary

The queue owns:

- synchronized occupancy checks;
- insertion and removal;
- capacity enforcement;
- FIFO behaviour;
- sleeping waits for blocking operations;
- wake-up after relevant state changes;
- release of blocked operations during closure.

The caller owns:

- choosing blocking or immediate behaviour;
- deciding whether to retry;
- coordinating several queues or events;
- choosing another action after an immediate failure.

## Consequences

### Positive

- simple callers can wait without creating an external waiting system;
- higher-level components can retain control through immediate
  operations;
- the waiting choice is visible at the call site;
- the queue remains responsible for atomic state transitions;
- one API can support both single-queue and multi-queue contexts.

### Negative

- four operations require separate documentation and verification;
- callers must define a policy after an immediate failure;
- Boolean results provide less lifecycle information than
  `op::status`;
- blocking on one queue remains unsuitable for some multi-resource
  coordination patterns.

### Accepted trade-off

The project accepts a larger API in exchange for letting each caller
choose whether queue backpressure should suspend the thread or return
control immediately.

## Current API Limitation

The current `try_push()` implementation reports failure when the queue
is full but does not yet represent closure through its Boolean result.

This is an API and implementation limitation, not a separate overload
policy. Before claiming that every insertion path rejects work after
closure, the immediate-insertion lifecycle behaviour must be made
explicit and tested.

Similarly, `try_pop()` returns `false` for an empty queue without
distinguishing whether the queue is open or closed.

A richer immediate-operation result may be considered if callers need
those distinctions.

## Validation

Tests should verify that:

- blocking insertion waits while an open queue is full;
- immediate insertion returns without waiting for capacity;
- blocking removal waits while an open queue is empty;
- immediate removal returns without waiting for an element;
- closure releases blocked operations;
- accepted buffered elements can be removed after closure;
- all successful operations preserve FIFO order and capacity.

## Reconsider When

Reconsider this decision when:

- timed operations become necessary;
- immediate operations require several failure reasons;
- multi-queue coordination becomes the dominant usage model;
- a higher-level scheduler assumes responsibility for all waiting;
- the Boolean result model becomes insufficient.

## Related Decisions

- ADR-0001: Use mutex-based synchronization
- ADR-0002: Use `std::deque` as the initial backing store
- ADR-0003: Keep queue identity stable