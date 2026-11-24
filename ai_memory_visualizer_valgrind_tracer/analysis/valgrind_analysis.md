# Valgrind Analysis

## 1. `heap_example`

- **Error Type:** Memory leak (`definitely lost: 6 bytes in 1 blocks`). Valgrind reports 1 error from 1 context.
- **Memory Object Involved:** The 6-byte heap block at `0x4a934d0` storing the string `"Alice\0"` (`alice->name`), allocated with `malloc` in `person_new` (`heap_example.c:21`), called from `main` (`heap_example.c:51`).
- **Cause / Lifetime Violation:** In `main`, `bob` is freed completely (`free(bob->name)` followed by `free(bob)`). However, `alice` is freed with `person_free_partial(alice)`, which only frees the `Person` struct (`0x4a93480`) and never frees `alice->name` (`0x4a934d0`). Once the struct is freed, the only pointer to `"Alice\0"` is lost, leaving the 6-byte block allocated and unreachable until program exit.
- **Heap Summary Note:** Valgrind reports 5 allocs and 4 frees (1,066 bytes total). Four allocations come from `alice` and `bob` (two structs + two name strings), and the extra 1,024-byte allocation/free is the internal `stdout` buffer created by `printf`.

## 2. `stack_example`

- **Error Type:** None (`0 errors from 0 contexts`, `0 bytes in 0 blocks` in use at exit).
- **Memory Object Involved:** None. No heap memory is allocated by user code.
- **Cause / Lifetime Behavior:** All variables (`local_int`, `local_buf`, `p_local`, `marker`) live on the stack. Stack frames are created automatically on function entry and destroyed on function exit, so no manual deallocation is needed and no lifetime violations occur.
- **Heap Summary Note:** The 1 alloc and 1 free of 1,024 bytes reported by Valgrind belong to `printf`'s internal output buffer, not to user code.

## 3. `aliasing_example`

- **Error Type:** Three use-after-free errors (`3 errors from 3 contexts`, 0 memory leaks):
  - **Invalid read of size 4** at `0x4a93488` (8 bytes inside the freed 20-byte block) when reading `b[2]` in `printf`.
  - **Invalid write of size 4** at `0x4a9348c` (12 bytes inside the freed 20-byte block) when executing `b[3] = 1234`.
  - **Invalid read of size 4** at `0x4a9348c` (12 bytes inside the freed 20-byte block) when reading `b[3]` in `printf`.
- **Memory Object Involved:** The 20-byte heap array of 5 `int`s allocated in `make_numbers` at `0x4a93480` (`b[2]` is at offset +8 bytes; `b[3]` is at offset +12 bytes).
- **Cause / Lifetime Violation:** `b = a` creates a pointer alias, so both `a` and `b` hold `0x4a93480`. Calling `free(a)` ends the lifetime of that 20-byte heap block and leaves both `a` and `b` as dangling pointers. Every subsequent read or write through `b` accesses freed memory (undefined behavior).
- **Heap Summary Note:** Valgrind reports 2 allocs and 2 frees (1,044 bytes total): 20 bytes for the `int` array and 1,024 bytes for `printf`'s internal buffer.

## 4. `crash_example`

- **Error Type:** Invalid write of size 4, followed by process termination via signal 11 (`SIGSEGV`). Valgrind reports 1 error from 1 context.
- **Valgrind Diagnostic:** `Address 0x0 is not stack'd, malloc'd or (recently) free'd` and `Access not within mapped region at address 0x0`.
- **Memory Object Involved:** The pointer `nums` in `main`, which holds `NULL` (`0x0`). No user heap block was allocated.
- **Cause / Lifetime Violation:** `allocate_numbers(0)` hits `if (n <= 0)` and returns `NULL` without calling `malloc`. `main` does not check whether `nums` is `NULL` before running `nums[0] = 42`, attempting to write 4 bytes to unmapped address `0x0`. The OS immediately terminates the process with `SIGSEGV` before `printf` or `free(nums)` is reached.
- **Heap Summary Note (`still reachable: 1,024 bytes`):** Valgrind reports 1 alloc and 0 frees (1,024 bytes). This is not a user memory leak—it is the internal `stdout` buffer allocated by the earlier `printf` calls. Because the program crashed with `SIGSEGV`, normal exit cleanup never ran to free it.