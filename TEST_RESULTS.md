# Validation

Compiled and tested on Linux using GCC with `-std=c11 -Wall -Wextra -pedantic -O0`.
The compiler reports the expected infinite-recursion warning for the deliberate buggy function.

| Test | Observed result |
| --- | --- |
| factorial(5) | 120 |
| factorial(10) | 3628800 |
| fibonacci(10) | 55; depth 10; 177 calls |
| fibonacci(20) | 6765; depth 20; 21891 calls |
| fibonacci(30) | 832040; depth 30; 2692537 calls |
| Intentional overflow | SIGSEGV (signal 11) in isolated process with 256 KiB stack and core dumps disabled |
| Safe recursion | Calls 0 through 10, then stops normally |
| Double callback | 2 4 6 8 10 |
| Square callback | 1 4 9 16 25 |
| Negate callback | -1 -2 -3 -4 -5 |
| Score event | Two registered callbacks receive 100 |
| Level event | Callback receives 2 |
| Health event | Callback receives 75 |

The full trace completed with 5,429,288 output lines. Fibonacci's total work grows exponentially; maximum active recursion depth grows linearly.

## Run

```sh
gcc stack_explorer.c -o stack_explorer
./stack_explorer
```

For readable summaries of large Fibonacci tests:

```sh
./stack_explorer --quiet-fibonacci
```

For the intentional crash (run separately):

```sh
./stack_explorer --overflow
```

The overflow call is opt-in instead of requiring edits to main. Keep test inputs small: the integer-returning factorial and Fibonacci functions are educational examples, not arbitrary-precision routines. A base case alone does not make an excessively large depth limit safe.
