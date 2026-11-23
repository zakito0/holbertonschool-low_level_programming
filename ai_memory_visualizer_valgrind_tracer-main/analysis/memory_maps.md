# Memory Maps and Lifetime Analysis

This document provides explicit step-by-step memory maps for the four C programs in `programs/`, distinguishing stack frames from heap allocations, tracking object lifetimes and pointer aliasing, and documenting a critical correction made to an AI-generated analysis.

---

## 1. `stack_example.c` — Recursion and Stack Frame Lifetimes

### Key Execution Points
* Entry and exit of `main`, `walk_stack` (`depth` 0 to 3), and `dump_frame` (`"enter"` and `"exit"` phases)[cite: 7].

### Memory Map

| Execution Point | Stack State (Top to Bottom) | Heap State | Lifetimes & Pointers |
| :--- | :--- | :--- | :--- |
| **1. `main()` entry** | `main`: return address, saved base pointer (no local variables)[cite: 7] | Empty (0 bytes)[cite: 7] | `main` frame lives until `return 0;`[cite: 7]. |
| **2. `walk_stack(0, 3)`** | `main` $\rightarrow$ `walk_stack(depth=0)`: `depth=0`, `max_depth=3`, `marker=0`[cite: 7] | Empty[cite: 7] | `marker` at depth 0 lives until `walk_stack(0, 3)` returns[cite: 7]. |
| **3. `dump_frame("enter", 0)`** | `main` $\rightarrow$ `walk_stack(0)` $\rightarrow$ `dump_frame`: `label` (ptr to `.rodata`), `depth=0`, `local_int=100`, `local_buf[16]="A\0..."`, `p_local=&local_int`[cite: 7] | Empty[cite: 7] | `p_local` aliases `&local_int` within the same stack frame[cite: 7]. Once `dump_frame` returns, its stack frame is popped and `local_int`, `local_buf`, and `p_local` cease to exist[cite: 7]. |
| **4. Max recursion (`depth=3`)** | `main` $\rightarrow$ `walk_stack(0)` (`marker=0`) $\rightarrow$ `walk_stack(1)` (`marker=10`) $\rightarrow$ `walk_stack(2)` (`marker=20`) $\rightarrow$ `walk_stack(3)` (`marker=30`)[cite: 7] | Empty[cite: 7] | Four distinct `marker` variables coexist simultaneously at decreasing stack addresses (`&marker` shifts downward by the frame size of `walk_stack` at each call)[cite: 7]. |
| **5. `dump_frame("exit", 3)`** | `main` $\rightarrow$ `walk_stack(0..3)` $\rightarrow$ `dump_frame`[cite: 7] | Empty[cite: 7] | Because the stack pointer returned to the exact same depth after `"enter"` returned, `dump_frame("exit", 3)` reuses the exact same virtual stack addresses as `dump_frame("enter", 3)`[cite: 7]. |
| **6. Stack unwinding (`depth=2` to `0`)** | Frames for `walk_stack(3)` down to `walk_stack(0)` are sequentially popped[cite: 7] | Empty[cite: 7] | Each `marker` dies as its respective `walk_stack` call exits[cite: 7]. |

---

## 2. `heap_example.c` — Nested Heap Allocations and Memory Leak

### Key Execution Points
* `person_new("Alice", 30)` and `person_new("Bob", 41)` (two `malloc` calls per person)[cite: 4].
* Full deallocation of `bob` (`free(bob->name)` then `free(bob)`)[cite: 4].
* Partial deallocation of `alice` via `person_free_partial(alice)`[cite: 4].

### Memory Map

| Execution Point | Stack State (`main`) | Heap State | Lifetimes & Aliasing |
| :--- | :--- | :--- | :--- |
| **1. `alice = person_new("Alice", 30)`** | `alice = 0x55...2a0` (8 bytes)<br>`bob = NULL` (8 bytes)[cite: 4] | **Block A1 (`0x55...2a0`, 16B):** `Person` struct (`name = 0x55...2c0`, `age = 30`)<br>**Block A2 (`0x55...2c0`, 6B):** `"Alice\0"`[cite: 4] | `person_new`'s stack frame (`p`, `len`, `i`) is destroyed on return, but Blocks A1 and A2 persist on the heap[cite: 4]. |
| **2. `bob = person_new("Bob", 41)`** | `alice = 0x55...2a0`<br>`bob = 0x55...2e0`[cite: 4] | Blocks A1, A2 +<br>**Block B1 (`0x55...2e0`, 16B):** `Person` struct (`name = 0x55...300`, `age = 41`)<br>**Block B2 (`0x55...300`, 4B):** `"Bob\0"`[cite: 4] | Four independent heap blocks exist; no pointer aliasing between `alice` and `bob`[cite: 4]. |
| **3. `free(bob->name); free(bob);`** | `alice = 0x55...2a0`<br>`bob = 0x55...2e0` (dangling)[cite: 4] | Blocks A1, A2 intact.<br>Block B2 (`0x55...300`) freed first, then Block B1 (`0x55...2e0`) freed[cite: 4]. | Freeing `bob->name` before `bob` avoids a use-after-free read of `bob->name`. Variable `bob` still holds `0x55...2e0` as a dangling pointer[cite: 4]. |
| **4. `person_free_partial(alice)`** | `alice = 0x55...2a0` (dangling)<br>`bob = 0x55...2e0` (dangling)[cite: 4] | **Block A1 (`0x55...2a0`) freed.**<br>**Block A2 (`0x55...2c0`, 6B `"Alice\0"`): STILL ALLOCATED (LEAKED)**[cite: 4] | `person_free_partial` frees `p` (Block A1) without first freeing `p->name` (Block A2). Once Block A1 is freed, the only pointer to `0x55...2c0` is lost, leaking 6 bytes until process termination[cite: 4]. |

---

## 3. `aliasing_example.c` — Pointer Aliasing and Use-After-Free

### Key Execution Points
* `a = make_numbers(5)` (heap allocation of 5 `int`s)[cite: 8].
* `b = a` (alias creation)[cite: 8].
* `free(a)` followed by read (`b[2]`) and write (`b[3] = 1234`)[cite: 8].

### Memory Map

| Execution Point | Stack State (`main`) | Heap State | Lifetimes & Aliasing |
| :--- | :--- | :--- | :--- |
| **1. `a = make_numbers(5)`** | `a = 0x55...6b0`<br>`b = NULL`<br>`n = 5`[cite: 8] | **Block H1 (`0x55...6b0`, 20B):** `[0, 11, 22, 33, 44]`[cite: 8] | Block H1 lifetime begins on the heap[cite: 8]. |
| **2. `b = a;`** | `a = 0x55...6b0`<br>`b = 0x55...6b0`<br>`n = 5`[cite: 8] | **Block H1 (`0x55...6b0`, 20B):** `[0, 11, 22, 33, 44]`[cite: 8] | **Aliasing:** `a` and `b` are distinct stack variables holding the exact same heap address (`0x55...6b0`). `a[i]` and `b[i]` refer to the exact same memory locations[cite: 8]. |
| **3. `free(a);`** | `a = 0x55...6b0` (dangling)<br>`b = 0x55...6b0` (dangling)<br>`n = 5`[cite: 8] | **Block H1 (`0x55...6b0`):** Deallocated (returned to `glibc` heap allocator freelist/tcache)[cite: 8]. | Block H1's lifetime ends. Because C passes `a` by value to `free()`, **both `a` and `b` still store `0x55...6b0`** and both become dangling pointers[cite: 8]. |
| **4. `b[2]` and `b[3] = 1234`** | `a = 0x55...6b0` (dangling)<br>`b = 0x55...6b0` (dangling)[cite: 8] | **Block H1 (`0x55...6b0`):** Unallocated chunk; `1234` overwrites freed heap memory[cite: 8]. | **Undefined Behavior (Use-After-Free):** Reading `b[2]` and writing `b[3]` accesses deallocated heap memory. It does not immediately segfault because the underlying virtual memory page remains mapped to the process, risking silent heap metadata corruption[cite: 8]. |

---

## 4. `crash_example.c` — Deterministic `NULL` Dereference

### Key Execution Points
* `nums = allocate_numbers(0)`[cite: 2].
* `nums[0] = 42`[cite: 2].

### Memory Map

| Execution Point | Stack State | Heap State | Lifetimes & Program Behavior |
| :--- | :--- | :--- | :--- |
| **1. `main()` entry** | `nums = 0x0` (`NULL`)<br>`n = 0`[cite: 2] | Empty[cite: 2] | `nums` does not point to any valid object[cite: 2]. |
| **2. `allocate_numbers(0)`** | `main` (`nums=0x0, n=0`) $\rightarrow$ `allocate_numbers` (`n=0, arr=0x0, i=0`)[cite: 2] | Empty (`if (n <= 0) return NULL;` executes before `malloc`)[cite: 2] | `allocate_numbers` frame is popped and returns `0x0` (`NULL`), assigned to `nums`[cite: 2]. |
| **3. `nums[0] = 42;`** | `nums = 0x0`<br>`n = 0`[cite: 2] | Empty[cite: 2] | `nums[0]` desugars to `*(nums + 0)`, attempting a 4-byte store to virtual address `0x0`. Because page `0x0` is unmapped in user space, the MMU triggers a page fault and the Linux kernel terminates the process with **`SIGSEGV` (Segmentation fault)** before `printf` or `free(nums)` is reached[cite: 2]. |

---

## 5. AI Evaluation and Manual Correction

### AI Error Identified (in `aliasing_example.c`)
* **What the AI claimed:** In step 3 (`free(a);`), the AI's initial memory map stated that `free(a)` sets `a = NULL` on the stack while leaving only `b` as a dangling pointer holding `0x55...6b0`[cite: 8].
* **Why it is technically wrong:** In C, function arguments are strictly passed **by value**. The signature of `free` is `void free(void *ptr);`—it receives a copy of the address stored in `a`, not a pointer-to-pointer (`int **`). Therefore, `free(a)` cannot modify the stack variable `a` in `main`'s stack frame.
* **How it was corrected:** Updated the stack state after `free(a);` to show that **both** `a` and `b` retain the heap address `0x55...6b0`, and **both** become dangling pointers pointing to deallocated heap memory (until `a` is explicitly reassigned or goes out of scope).
