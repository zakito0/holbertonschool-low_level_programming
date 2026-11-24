# Valgrind Analysis

## 1. `heap_example`

- **Error Type:** Memory leak due to lost ownership (`definitely lost: 6 bytes in 1 blocks`, 1 error from 1 context).
- **Memory Object Involved:** The 6-byte heap block at `0x4a934d0` holding `"Alice\0"` (`alice->name`), allocated via `malloc` in `person_new` (`heap_example.c:21`), called from `main` (`heap_example.c:51`).
- **Cause / Lifetime Violation:** `main` owns the `Person` structs and their `name` strings returned by `person_new`, and is responsible for freeing both. While `bob` is freed completely (`free(bob->name)` then `free(bob)`), `alice` is passed to `person_free_partial(alice)`, which frees only the `Person` struct (`0x4a93480`) and never frees `alice->name` (`0x4a934d0`). Once the struct is freed, the only pointer to `"Alice\0"` is lost, leaving the 6-byte block unreachable until program exit.
- **Heap Summary Note:** Valgrind reports 5 allocs and 4 frees (1,066 bytes). Four allocations belong to `alice` and `bob` (two structs + two name strings); the extra 1,024-byte alloc/free is `glibc`'s internal `stdout` buffer used by `printf`.

## 2. `stack_example`

- **Error Type:** None (`0 errors from 0 contexts`, `0 bytes in 0 blocks` in use at exit).
- **Memory Object Involved:** None. No heap memory is allocated by user code.
- **Cause / Lifetime Behavior:** All variables (`local_int`, `local_buf`, `p_local`, `marker`) live on the stack. Stack frames are created on function entry and automatically destroyed on function return, so no manual `free` is required and no lifetime violations occur.
- **Heap Summary Note:** The 1 alloc and 1 free of 1,024 bytes belong to `printf`'s internal output buffer, not user code.

## 3. `aliasing_example`

- **Error Type:** Three use-after-free errors (`3 errors from 3 contexts`, 0 leaks):
  - **Invalid read of size 4** at `0x4a93488` (+8 bytes inside the freed 20-byte block) when reading `b[2]` in `printf`.
  - **Invalid write of size 4** at `0x4a9348c` (+12 bytes inside the freed 20-byte block) during `b[3] = 1234`.
  - **Invalid read of size 4** at `0x4a9348c` (+12 bytes inside the freed 20-byte block) when reading `b[3]` in `printf`.
- **Memory Object Involved:** The 20-byte heap array of 5 `int`s allocated in `make_numbers` at `0x4a93480`.
- **Cause / Lifetime Violation:** `b = a` creates a pointer alias, so both `a` and `b` hold `0x4a93480`. Calling `free(a)` ends the lifetime of the 20-byte block and leaves both `a` and `b` as dangling pointers. Every subsequent read or write through `b` accesses freed memory (undefined behavior).
- **Heap Summary Note:** Valgrind reports 2 allocs and 2 frees (1,044 bytes): 20 bytes for the `int` array and 1,024 bytes for `printf`'s internal buffer.

## 4. `crash_example`

- **Error Type:** Invalid write of size 4 (NULL pointer dereference), causing process termination via signal 11 (`SIGSEGV`, 1 error from 1 context).
- **Valgrind Diagnostic:** `Address 0x0 is not stack'd, malloc'd or (recently) free'd` and `Access not within mapped region at address 0x0` at `crash_example.c:32`.
- **Memory Object Involved:** The stack pointer `nums` in `main`, which holds `NULL` (`0x0`). No user heap block was allocated.
- **Cause / Lifetime Violation:** `allocate_numbers(0)` hits `if (n <= 0)` and returns `NULL` without calling `malloc`. `main` does not check for `NULL` before executing `nums[0] = 42`, attempting a 4-byte write to unmapped address `0x0`. The OS immediately terminates the process with `SIGSEGV` before `printf` or `free(nums)` is reached.
- **Heap Summary Note (`still reachable: 1,024 bytes`):** Valgrind reports 1 alloc and 0 frees (1,024 bytes). This is not a user memory leak—it is `printf`'s internal `stdout` buffer. Because the process crashed abruptly with `SIGSEGV`, standard exit cleanup never ran to free it.

## 5. AI Evaluation and Manual Correction

- **AI Claim (`crash_example`):** The initial AI analysis stated that no heap memory was allocated at all and omitted Valgrind's `HEAP SUMMARY` and `LEAK SUMMARY`.
- **Why It Is Partially Incorrect:** Although user code never reaches `malloc`, Valgrind reports `total heap usage: 1 allocs, 0 frees, 1,024 bytes allocated` and `still reachable: 1,024 bytes in 1 blocks`.
- **Correction:** That 1,024-byte allocation is `glibc`'s internal `stdout` buffer created by the earlier `printf` calls. The `SIGSEGV` crash skips normal C runtime exit cleanup, leaving the buffer `still reachable` at exit. It is neither "0 allocations" nor a user-code leak.