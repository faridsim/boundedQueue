# ADR-0003: Keep Queue Identity Stable by Disabling Copy and Move

## Status

Accepted

## Date

2026-07-29

---

# Context

Boundflow is a concurrent queue that owns synchronization state.

A queue object contains:

- stored elements;
- mutex;
- condition variables;
- closed state;
- relationships with threads that may currently be waiting.

The public type must define whether queue objects can be copied or moved.

The compiler-generated copy and move operations are not appropriate
because they cannot determine the intended meaning of transferring
synchronization state.

---

# Decision Drivers

The decision is based on:

## Synchronization ownership

The queue is not only a container of elements.

It is also a synchronization object.

Copying or moving requires defining what happens to:

- mutex ownership;
- waiting threads;
- condition variables;
- lifecycle state;
- buffered elements.

---

## Clear object semantics

Users should understand whether two queue objects represent:

- independent queues;
- shared queues;
- transferred ownership;
- snapshots of previous state.

Ambiguous semantics create correctness risks.

---

## Prevent invalid usage

The type system should prevent operations that have no well-defined
meaning.

---

# Options Considered

---

# Option 1: Allow Copy Construction

Example:

```cpp
queue<int> q2 = q1;