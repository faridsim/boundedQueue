# ADR-0002: Use std::deque as the Initial Backing Store

## Status

Accepted

## Date

2026-07-29

## Context

Boundflow requires an internal FIFO representation that supports adding
elements at the back and removing elements from the front.

The expected queue capacity is relatively small, approximately on the
order of the number of worker threads or processor cores. Allocation
efficiency is therefore not currently a primary requirement.

Storage still requires a deliberate choice because different
representations can have different allocation overhead, startup cost,
and suitability when capacity grows.

## Decision Drivers

- support FIFO insertion and removal naturally;
- remain sufficient for the expected small capacity;
- keep the queue implementation straightforward;
- avoid optimizing storage before storage becomes a demonstrated
  problem;
- preserve the option to change the private representation later.

## Considered Options

### Ring buffer

A ring buffer was considered the storage-optimal alternative.

It becomes more attractive when fixed-capacity storage efficiency or
allocation behaviour is an important requirement.

For the current queue, capacity is expected to remain small and
allocation efficiency is not a priority. Storage optimality therefore
does not justify requiring a ring-buffer implementation at this stage.

### Linked list

A linked list could support the required FIFO behaviour and would be
adequate for the expected capacity.

No requirement was identified that made it preferable to a deque.

### `std::deque<T>`

A deque directly supports insertion at the back and removal from the
front.

It is sufficient for the expected queue size and keeps storage concerns
secondary to the more important synchronization and lifecycle work.

## Decision

Use `std::deque<T>` as the initial backing store.

Queue capacity remains a logical constraint enforced by queue
operations. Selecting a deque does not mean it is universally better
than a ring buffer; it means it is sufficient for the current
requirements with less need for a custom storage implementation.

## Consequences

### Positive

- the representation maps directly to FIFO operations;
- the storage implementation remains small and readable;
- the project can focus on synchronization, blocking, and closure;
- the backing store remains private and replaceable;
- the representation is sufficient for the expected capacity.

### Negative

- the decision does not provide storage-optimal or fixed-allocation
  guarantees;
- a different representation may become appropriate if capacity grows;
- storage behaviour may need to be revisited for another workload.

### Accepted trade-off

The project accepts a representation that is sufficient rather than
storage-optimal in exchange for a simpler initial implementation.

## Validation

Tests should verify properties of the queue contract rather than
properties of the selected container:

- FIFO order;
- bounded capacity;
- correct full and empty behaviour;
- preservation of accepted elements;
- no mutation after a failed immediate operation.

## Reconsider When

Reconsider this decision when:

- queue capacity becomes substantially larger;
- allocation efficiency becomes a requirement;
- startup or storage overhead becomes significant;
- measurement identifies the backing store as a performance limitation;
- a fixed-capacity ring buffer provides a demonstrated benefit.

A replacement must preserve the existing public FIFO, capacity,
blocking, and lifecycle behaviour.

## Related Decisions

- ADR-0001: Use mutex-based synchronization
- ADR-0003: Keep queue identity stable
- ADR-0004: Expose blocking and immediate operations