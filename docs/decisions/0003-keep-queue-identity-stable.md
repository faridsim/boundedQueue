# ADR-0003: Keep Queue Identity Stable

## Status

Accepted

## Date

2026-07-29

## Context

A `bounded_queue<T>` owns both buffered elements and synchronization
state.

Its synchronization state includes a mutex, condition variables, and
the relationships between the queue and threads that may currently be
using or waiting on it.

Ordinary copy or move syntax would require a clearly defined answer to
questions such as:

- what queue state is copied or transferred;
- whether the new queue is independent;
- what happens to synchronization state;
- what happens to threads interacting with the original object.

No such copy, snapshot, or transfer contract is required for the current
queue.

The compiler should not be allowed to imply semantics that the project
has not defined.

## Decision Drivers

- keep synchronization ownership unambiguous;
- prevent unsupported object operations at compile time;
- avoid pretending that buffered data and synchronization state have
  ordinary value semantics;
- keep one queue instance associated with one synchronization state;
- avoid introducing snapshot or transfer behaviour without a concrete
  requirement.

## Considered Options

### Define copy and move semantics

This would require an explicit contract for copying or transferring
buffered elements and synchronization-related state.

Such behaviour is not required by the current project, and no clear
semantics have been adopted.

### Disable copy and move operations

Construct each queue in its intended location and keep that queue object
stable throughout its lifetime.

Callers share access to that object rather than copying or moving the
queue itself.

## Decision

Disable:

- copy construction;
- copy assignment;
- move construction;
- move assignment.

The current declarations are:

```cpp
bounded_queue(const bounded_queue&) = delete;
bounded_queue(bounded_queue&&) = delete;
bounded_queue& operator=(const bounded_queue&) = delete;
bounded_queue& operator=(bounded_queue&&) = delete;
```

This is the only code excerpt needed because the decision concerns the
type interface itself.

## Consequences

### Positive

- unsupported copy and move operations fail at compile time;
- synchronization ownership remains clear;
- one queue object continues to represent one queue state;
- the restriction is visible in the public interface;
- no accidental copy or transfer semantics are implied.

### Negative

- the queue cannot be used directly where a copyable or movable value is
  required;
- ownership transfer requires another ownership mechanism;
- callers must manage the queue’s location and lifetime explicitly.

### Accepted trade-off

The project accepts reduced value-type flexibility in exchange for
clear synchronization ownership and an explicit object contract.

## Validation

Compile-time type-trait tests should confirm that the queue is neither
copyable nor movable.

Runtime tests are not required to validate operations that must fail
during compilation.

## Reconsider When

Reconsider this decision when the project gains a concrete requirement
for:

- queue snapshots;
- transferring ownership of an inactive queue;
- a shared-state handle;
- storage inside an abstraction that requires movable values.

Any change must first define the observable semantics of the new
operation.

## Related Decisions

- ADR-0001: Use mutex-based synchronization
- ADR-0002: Use `std::deque` as the initial backing store
- ADR-0004: Expose blocking and immediate operations