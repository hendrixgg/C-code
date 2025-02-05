# CISC/CMPE 223 Assignment 1 Problem 8
Completed by Hendrix Gryspeerdt.

## Problem Statemet
Write a program (in C, C++, Java, or Python) that takes a finite automaton description (in the 5-tuple format) and a string as input, and determines whether the automaton accepts the string.

```
fa_description = {
    "states": {"q0", "q1", "q2"} , 
    "alphabet": {"0", "1"} , 
    "transitions": {
        ("q0", "0"): "q1", 
        ("q0", "1"): "q0", 
        ("q1", "0"): "q2", 
        ("q1", "1"): "q0", 
        ("q2", "0"): "q2", 
        ("q2", "1"): "q2", 
    }, 
    "start_state": "q0" , 
    "accept_states": {"q2"}, 
}
input_strings = ["0", "01", "001", "0001", "111", "10", "000"]
```

Expected Outputs: 
```
Input: 0 -> Accepted: False
Input: 01 -> Accepted: False
Input: 001 -> Accepted: True
Input: 0001 -> Accepted: True
Input: 111 -> Accepted: False
Input: 10 -> Accepted: False
Input: 000 -> Accepted: True
```

## Solution
Programmed the solution in C.

Data structure and interface located in `include/discrete_finite_automaton.h`, implementation of dfa functionality in `src/discrete_finite_automaton.h`, instantiation and testing code in `test/test_dfa.c`.

Compile the program:
```
gcc -o test_dfa test/test_dfa.c src/discrete_finite_automaton.c -I./include
```

Run the program:
```
./test_dfa.exe
```
