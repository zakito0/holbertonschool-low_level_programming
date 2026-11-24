# Memory Maps and Lifetime Analysis

This document provides step-by-step memory maps for the four C programs in `programs/`, distinguishing stack frames from heap allocations, tracking object lifetimes, ownership, and pointer aliasing, and documenting a correction to the AI-generated analysis. *(Hexadecimal addresses shown below are illustrative.)*

## 1. `stack_example.c` — Recursion and Stack Frame Lifetimes

### Key Execution Points

- Entry and exit of `main`
- Entry and exit of `walk_stack` for depths 0–3
- Entry and exit of `dump_frame`

### Memory Map

| Execution Point | Stack State | Heap State | Lifetimes & Pointers |
|---|---|---|---|
| **1. `main()` entry** | `main`: return address, saved base pointer; no local variables yet | Empty | `main` frame lives until `return 0;` |
| **2. `walk_stack(0, 3)`** | `main` → `walk_stack(depth=0)`: `depth=0`, `max_depth=3`, `marker=0` | Empty | `marker` at depth 0 lives until `walk_stack(0, 3)` returns |
| **3. `dump_frame("enter", 0)`** | `main` → `walk_stack(0)` → `dump_frame`: `label` points to `.rodata`, `depth=0`, `local_int=100`, `local_buf[16]="A\0..."`, `p_local=&local_int` | Empty | `p_local` aliases `local_int`. When `dump_frame` returns, its stack frame is destroyed and all its local objects become invalid |
| **4. Maximum recursion (`depth=3`)** | `main` → `walk_stack(0)` → `walk_stack(1)` → `walk_stack(2)` → `walk_stack(3)`; markers are `0`, `10`, `20`, `30` | Empty | Four distinct `marker` objects coexist, each owned by its own stack frame |
| **5. `dump_frame("exit", 3)`** | Same active `walk_stack` frames plus a new `dump_frame` frame | Empty | The new `dump_frame` invocation may reuse the same stack addresses as the earlier invocation, though exact address reuse is an implementation detail |
| **6. Stack unwinding** | `walk_stack(3)` through `walk_stack(0)` return sequentially | Empty | Each `marker` object ceases to exist as its stack frame is popped; stack memory never survives function return |

---

## 2. `heap_example.c` — Nested Heap Allocations and Memory Leak

### Key Execution Points

- `person_new("Alice", 30)`
- `person_new("Bob", 41)`
- Full deallocation of `bob`
- Partial deallocation of `alice`

### Memory Map

| Execution Point | Stack State (`main`) | Heap State | Lifetimes, Ownership & Aliasing |
|---|---|---|---|
| **1. `alice = person_new("Alice", 30)`** | `alice = 0x55...2a0`, `bob = NULL` | **A1:** `Person` struct at `0x55...2a0` (`name = 0x55...2c0`, `age = 30`).<br>**A2:** `"Alice\0"` at `0x55...2c0` | Local variables inside `person_new` disappear on return, but A1 and A2 survive on the heap. Ownership transfers to `main`, which must free both A2 and A1 |
| **2. `bob = person_new("Bob", 41)`** | `alice = 0x55...2a0`, `bob = 0x55...2e0` | A1, A2, plus **B1:** `Person` at `0x55...2e0` (`name = 0x55...300`, `age = 41`).<br>**B2:** `"Bob\0"` at `0x55...300` | Four independent heap allocations exist; `main` owns all four |
| **3. `free(bob->name); free(bob);`** | `alice = 0x55...2a0`, `bob = 0x55...2e0` (dangling) | A1 and A2 remain. B2 is freed, then B1 is freed | `bob` is freed in the right order (nested string before struct). `bob` still holds the old address, making it a dangling pointer |
| **4. `person_free_partial(alice)`** | `alice = 0x55...2a0` (dangling), `bob = 0x55...2e0` (dangling) | A1 is freed. **A2 remains allocated** | The only pointer to A2 lived inside A1. Freeing A1 without freeing `alice->name` first causes lost ownership, leaking A2 |

---

## 3. `aliasing_example.c` — Pointer Aliasing and Use-After-Free

### Key Execution Points

- `a = make_numbers(5)`
- `b = a`
- `free(a)`
- Access through `b`

### Memory Map

| Execution Point | Stack State (`main`) | Heap State | Lifetimes, Ownership & Aliasing |
|---|---|---|---|
| **1. `a = make_numbers(5)`** | `a = 0x55...6b0`, `b = NULL`, `n = 5` | **H1:** 5 `int`s at `0x55...6b0`: `[0, 11, 22, 33, 44]` | H1's lifetime begins with `malloc`; `main` owns H1 and must free it once |
| **2. `b = a`** | `a = 0x55...6b0`, `b = 0x55...6b0`, `n = 5` | H1: `[0, 11, 22, 33, 44]` | `a` and `b` are distinct stack variables aliasing the same heap allocation H1 |
| **3. `free(a)`** | `a = 0x55...6b0`, `b = 0x55...6b0`, `n = 5` | H1 is deallocated | `free` receives a copy of the address and modifies neither `a` nor `b`. H1's lifetime ends and both pointers become dangling |
| **4. `b[2]` and `b[3] = 1234`** | `a` and `b` still hold the former H1 address | H1 is no longer allocated | Both operations are use