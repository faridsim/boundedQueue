# ADR-0002: Use std::deque as the Initial Backing Storage

## Status

Accepted

## Date

2026-07-29

---

# Context

Boundflow requires an internal storage structure for maintaining
elements in FIFO order.

The storage choice affects:

- memory behaviour;
- allocation strategy;
- implementation complexity;
- future optimization options;
- interaction with element types.

The queue exposes a bounded interface:

- maximum logical capacity;
- FIFO insertion;
- FIFO removal.

The internal container is an implementation detail and should support
the required behaviour without unnecessarily increasing complexity.

---

# Decision Drivers

The storage design is evaluated using:

## FIFO suitability

The container must efficiently support:

- insertion at the back;
- removal from the front.

---

## Correctness simplicity

The storage mechanism should make queue invariants easy to reason about:

- ordering;
- size tracking;
- shutdown behaviour.

---

## Maintainability

The implementation should remain understandable before introducing
performance-specific optimizations.

---

## Future optimization path

The selected storage should not prevent replacing the implementation
later if measurements justify it.

---

# Options Considered

---

# Option 1: std::deque<T>

## Description

Use the standard library double-ended queue as internal storage.

Example:

```cpp
std::deque<T> content;