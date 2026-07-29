# ADR-0001: Use Mutex-Based Synchronization Instead of Lock-Free Design

## Status

Accepted

## Date

2026-07-29

---

# Context

Boundflow is a bounded blocking queue designed for producer-consumer
communication.

The queue requires:

- bounded capacity;
- blocking producers when capacity is unavailable;
- blocking consumers when elements are unavailable;
- explicit shutdown;
- predictable wake-up behaviour;
- understandable correctness guarantees.

A synchronization strategy must be selected.

The main alternatives considered are:

1. mutex + condition variables;
2. lock-free queue design.

---

# Decision Drivers

The decision is based on:

## Correctness

The queue must clearly define:

- state transitions;
- shutdown behaviour;
- waiting conditions;
- ownership of shared state.

---

## Maintainability

The implementation should be understandable and reviewable.

Concurrency bugs are difficult to diagnose, therefore reducing unnecessary
complexity is valuable.

---

## Requirement Fit

The current requirements do not include:

- measured lock contention problems;
- hard latency requirements;
- lock-free progress requirements.

---

## Evidence-Based Optimization

Performance improvements should be introduced when measurements show a
real bottleneck.

Optimizing before identifying the bottleneck increases complexity without
guaranteed benefit.

---

# Options Considered

---

# Option 1: Mutex + Condition Variables

## Description

Protect queue state with a mutex.

Use condition variables to coordinate:

- producers waiting for capacity;
- consumers waiting for elements.

Example state protection:

```cpp
std::mutex mutex;
std::condition_variable not_empty;
std::condition_variable not_full;