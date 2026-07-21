# Embedded C Circular Buffer

A step-by-step, safety-oriented circular-buffer lesson for Ubuntu 24.04 and VS Code.

## Install tools

```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build gdb git clang-format cppcheck
```

Install VS Code, open this folder, and accept the recommended C/C++ and CMake Tools extensions.

```bash
cd lessons/circular-buffer
code .
```

## Build and test

```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

Run host sanitizers:

```bash
cmake --preset sanitized
cmake --build --preset sanitized
ctest --preset sanitized
```

In VS Code, `Ctrl+Shift+B` builds. Run the test tasks from the Command Palette.

## What this version guarantees

- Caller-owned fixed storage; no heap allocation.
- O(1) push, pop, peek, and reset operations.
- Full buffers reject new data instead of silently overwriting unread data.
- Explicit status codes for null arguments, empty/full states, and detected corruption.
- Runtime validation of index/count invariants.
- Strict compiler warnings and boundary tests.

## Core invariant

```text
write_index = (read_index + count) modulo capacity
```

The implementation computes this relation without an overflowing addition.

## Critical-system limitation

This is educational code, not a certified safety component. It is not safe for simultaneous ISR and main-loop access. The next lesson should design a single-producer/single-consumer variant with explicit ownership and platform-specific memory-order analysis. Do not attempt to fix concurrency by only adding `volatile`.

## Suggested debugging exercise

Set a breakpoint in `test_wrap_around`, then watch `read_index`, `write_index`, and `count` while stepping through push and pop operations.
