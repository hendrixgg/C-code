# Simple [Push Down Automaton](https://en.wikipedia.org/wiki/Pushdown_automaton) Simulation

## Why does this exist?
This simple program was created by Hendrix Gryspeerdt for Problem 4 of Assignment 2 for the course CISC/CMPE223 Software Specifications at Queen's University on 2025-03-20.

## What is this?

In [`main.c`](./main.c) there is a function called `pda_accept` which takes in a pointer to a null-terminated string of characters and returns whether it belongs to the [context-free language](https://en.wikipedia.org/wiki/Context-free_language) defined by the following [context-free grammar](https://en.wikipedia.org/wiki/Context-free_grammar) in [Backus-Naur form](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form) below:

```
<S> ::== "c" | "1" <S> "1" | "0" <S> "0"
```

## How to compile and run

Compile using [`gcc`](https://gcc.gnu.org/) (the flags are optional)
```
gcc main.c -Wall -Wextra -Wpedantic
```

Run the executable file produced (`a` or `a.exe`).

## Modifying the test cases

You can change the test cases by modifying the array in `main`:
```c
int main() {
    const struct test_case test_cases[] = {
        {"10110c01101", true},
        {"10010c01101", false},
        {"23498f3", false},
        {"000cc000", false},
        {"01010101c01010101", false},
        {"01010101c10101010", true},
        {"0110001c1000110", true},
        // you can add more test cases here
    };
    test_pda_accept(stdout, test_cases, sizeof(test_cases) / sizeof(test_cases[0]));
    return 0;
}
```