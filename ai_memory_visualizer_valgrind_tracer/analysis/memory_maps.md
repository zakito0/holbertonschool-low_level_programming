# Memory Maps and Lifetime Analysis

This document provides step-by-step memory maps for the four C programs in `programs/`, distinguishing stack frames from heap allocations, tracking object lifetimes and pointer aliasing, and documenting a correction to the AI-generated analysis.

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
| **3. `dump_frame("enter", 0)`** | `main` → `walk_stack(0)` → `dump_frame`: `label` points to `.rodata`, `depth=0`, `local_int=100`, `local_buf[16]="A\0..."`, `p_local=&local_int` | Empty | `p_local` aliases `local_int`. When `dump_frame` returns, its stack frame is destroyed and these local objects cease to exist |
| **4. Maximum recursion (`depth=3`)** | `main` → `walk_stack(0)` → `walk_stack(1)` → `walk_stack(2)` → `walk_stack(3)`; markers are `0`, `10`, `20`, `30` | Empty | Four distinct `marker` objects coexist. Each belongs to a different stack frame |
| **5. `dump_frame("exit", 3)`** | Same active `walk_stack` frames plus a new `dump_frame` frame | Empty | The new `dump_frame` invocation may reuse the same stack addresses as the earlier invocation, but exact address reuse is an implementation detail |
| **6. Stack unwinding** | `walk_stack(3)` through `walk_stack(0)` return sequentially | Empty | Each corresponding `marker` object ceases to exist when its function invocation returns |

---

## 2. `heap_example.c` — Nested Heap Allocations and Memory Leak

### Key Execution Points

- `person_new("Alice", 30)`
- `person_new("Bob", 41)`
- Full deallocation of `bob`
- Partial deallocation of `alice`

### Memory Map

| Execution Point | Stack State (`main`) | Heap State | Lifetimes & Aliasing |
|---|---|---|---|
| **1. `alice = person_new("Alice", 30)`** | `alice = 0x55...2a0`, `bob = NULL` | **A1:** `Person` struct at `0x55...2a0`, containing `name = 0x55...2c0`, `age = 30`.<br>**A2:** `"Alice\0"` at `0x55...2c0` | The local variables inside `person_new` disappear when it returns, but A1 and A2 remain allocated |
| **2. `bob = person_new("Bob", 41)`** | `alice = 0x55...2a0`, `bob = 0x55...2e0` | A1 and A2 plus **B1:** `Person` at `0x55...2e0`, `name = 0x55...300`, `age = 41`.<br>**B2:** `"Bob\0"` at `0x55...300` | Four independent heap allocations exist |
| **3. `free(bob->name); free(bob);`** | `alice = 0x55...2a0`, `bob = 0x55...2e0` (dangling) | A1 and A2 remain. B2 is freed, followed by B1 | `bob` still contains the old address after `free(bob)`, so it is a dangling pointer |
| **4. `person_free_partial(alice)`** | `alice = 0x55...2a0` (dangling), `bob = 0x55...2e0` (dangling) | A1 is freed. **A2 remains allocated** | The pointer to A2 was stored only inside A1. Once A1 is freed, A2 becomes unreachable and is leaked |

---

## 3. `aliasing_example.c` — Pointer Aliasing and Use-After-Free

### Key Execution Points

- `a = make_numbers(5)`
- `b = a`
- `free(a)`
- Access through `b`

### Memory Map

| Execution Point | Stack State (`main`) | Heap State | Lifetimes & Aliasing |
|---|---|---|---|
| **1. `a = make_numbers(5)`** | `a = 0x55...6b0`, `b = NULL`, `n = 5` | **H1:** 5 `int`s at `0x55...6b0`: `[0, 11, 22, 33, 44]` | H1's lifetime begins with the allocation |
| **2. `b = a`** | `a = 0x55...6b0`, `b = 0x55...6b0`, `n = 5` | H1: `[0, 11, 22, 33, 44]` | `a` and `b` are distinct pointer variables that point to the same allocation |
| **3. `free(a)`** | `a = 0x55...6b0`, `b = 0x55...6b0`, `n = 5` | H1 is deallocated | `free` receives a copy of the pointer value. It does not change either `a` or `b`; both become dangling pointers |
| **4. `b[2]` and `b[3] = 1234`** | `a` and `b` still contain the former H1 address | H1 is no longer allocated | Both operations have undefined behavior because they access memory after its lifetime has ended |

---

## 4. `crash_example.c` — `NULL` Dereference

### Key Execution Points

- `nums = allocate_numbers(0)`
- `nums[0] = 42`

### Memory Map

| Execution Point | Stack State | Heap State | Lifetimes & Program Behavior |
|---|---|---|---|
| **1. `main()` entry** | `nums = NULL`, `n = 0` | Empty | `nums` does not point to an allocated object |
| **2. `allocate_numbers(0)`** | `main` calls `allocate_numbers` with `n=0` | Empty | Because `n <= 0`, the function returns `NULL` without allocating memory. Its stack frame then disappears |
| **3. `nums[0] = 42`** | `nums = NULL`, `n = 0` | Empty | `nums[0]` attempts to access memory through a null pointer. This is undefined behavior and commonly results in `SIGSEGV` on Linux. `printf` and `free(nums)` are not reached if the process is terminated by the fault |

---

## 5. AI Evaluation and Manual Correction

### Error in `aliasing_example.c`

The initial analysis incorrectly claimed that:

> `free(a)` sets `a = NULL`

This is incorrect.

In C, function arguments are passed by value. `free(a)` passes a copy of the pointer value stored in `a` to:

```c
void free(void *ptr);
```

`free` therefore cannot modify the `a` variable in `main`.

After:

```c
free(a);
```

both variables still contain the same address:

```text
a = 0x55...6b0
b = 0x55...6b0
```

However, the allocation at that address has ended, so **both pointers are dangling**.

The corrected interpretation is therefore:

```text
Before free:
a ──┐
    ├──> heap allocation
b ──┘

After free(a):
a ──┐
    ├──> deallocated memory
b ──┘
```

Any subsequent dereference through either pointer has undefined behavior.
