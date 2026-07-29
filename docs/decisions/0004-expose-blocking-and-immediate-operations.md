# ADR-0004: Expose Blocking and Immediate Operations as Separate APIs

## Status

Accepted

## Date

2026-07-29

---

# Context

A concurrent queue must define how callers interact with unavailable
resources.

Examples:

A producer may attempt to insert into a full queue.

A consumer may attempt to remove from an empty queue.

The queue could decide internally to always block, always fail, or expose
different behaviours.

However, different applications require different policies.

Examples:

- a worker thread may wait until capacity becomes available;
- an event loop may need immediate feedback;
- a scheduler may want to choose another queue;
- a system with deadlines may need timeout handling.

The queue abstraction should provide synchronization guarantees without
taking ownership of application-level decisions.

---

# Decision Drivers

The decision is based on:

---

## Separation of Responsibilities

The queue should manage:

- thread safety;
- state transitions;
- capacity;
- synchronization.

The caller should manage:

- retry policy;
- backoff strategy;
- dropping work;
- deadlines;
- coordination between multiple queues.

---

## Explicit Behaviour

The API should make waiting behaviour visible.

A caller should be able to understand whether an operation:

- may block;
- returns immediately.

---

## Composability

Higher-level systems may combine multiple queues or scheduling policies.

The queue should not force one waiting strategy.

---

# Options Considered

---

# Option 1: Blocking Operations Only

Example:

```cpp
queue.push(item);
queue.pop(item);