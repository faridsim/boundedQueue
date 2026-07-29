# Boundflow Queue Design

## 1. Purpose

Boundflow is a bounded thread-safe FIFO queue implemented in modern C++.

The purpose of this component is not only to store elements safely
between producers and consumers, but to define explicit concurrency
semantics:

- when operations block;
- when operations return immediately;
- how shutdown behaves;
- what correctness guarantees exist;
- what responsibilities belong to the queue and what responsibilities
  remain with the caller.

The design prioritizes:

- correctness;
- explicit behavioural contracts;
- understandable synchronization;
- testable guarantees;
- maintainable engineering decisions.

Performance optimizations that increase complexity should only be
introduced when supported by measurements.

---

# 2. Design Goals

The queue should provide:

- bounded FIFO storage;
- safe concurrent access;
- deterministic state transitions;
- explicit shutdown semantics;
- clear failure behaviour;
- verifiable correctness through tests.

The queue should make important behaviours observable:

- successful insertion;
- successful removal;
- blocking;
- wake-up;
- shutdown;
- rejected operations.

---

# 3. Non-Goals

Boundflow does not attempt to provide:

- lock-free progress guarantees;
- automatic retry policies;
- scheduling decisions;
- work stealing;
- priority ordering;
- application-level cancellation;
- fairness guarantees between competing threads.

These concerns belong to higher-level components.

---

# 4. Responsibility Boundary

A queue abstraction should provide safe storage and coordination,
not application policy.

## Queue responsibilities

The queue owns:

- protecting internal state;
- enforcing capacity limits;
- maintaining FIFO ordering;
- managing synchronization;
- enforcing closed-state rules;
- waking blocked operations;
- providing operation results.

## Caller responsibilities

The caller owns:

- deciding whether waiting is acceptable;
- deciding whether to retry;
- choosing retry frequency;
- deciding whether work can be dropped;
- coordinating multiple queues;
- implementing deadlines or cancellation.

The queue provides different APIs so callers can choose the required
behaviour.

Examples:

```cpp
push()