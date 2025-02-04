# CISC/CMPE 223 Assignment 1 Problem 7
## Problem Statement
### (a)
Design and draw a DFA over the alphabet {a, b} that accepts all strings (given in the table for question 7.c i.e., string length will not exceed the maximum length in the table) where the absolute difference between the number of a’s ($n_a$) and b’s ($n_b$) is at most 3, i.e., $|n_a − n_b| \leq 3$.
### (b)
Write a simulator function that simulates the state transitions of your DFA to accept or reject strings.
### (c)
Complete the table below by running your simulator on the given input strings and determining whether each string is accepted or rejected.

| Input String     | Accept/Reject |
| ---------------- | ------------- |
| aaa              | 
| abab | 
| aaaabbb |
|aaaaaaa |
|bbbbbbb |
|aaaaaaabbbbbbbb |
|bbbb |  
|aaaabbbbaaaa |
|bbbaaaabbbb |
|aaaaaaaabbbbbbbb |
|abababab |
|ababababab |
|aaabbbbaa |
|abbbaaaa |
|bbbaaabbb |
|(empty string)|
| a|     
| b |    
|aaabbb |
|abababababab|


## Solution
### (a)
Noticing that the maximum length string in the above table is length 16.

Formalization of the problem: design a DFA over the alphabet $\Sigma = \{a, b\}$ such that it recognizes the language $L$, where 

$$L=\{w\in\Sigma^*| |w|\leq 16 \wedge |\text{(\# of a's in w)} - \text{(\# of b's in w)}|\leq 3\}$$

See #reMarkable for drawing.
### (b)
Implementation is defined in `dfa_implementation.c`.
### (c)
Results of testing the code on the input strings in the table:

|Input String|Accept/Reject|
|------------|-------------|
|aaa|Accept|
|abab|Accept|
|aaaabbb|Accept|
|aaaaaaa|Reject|
|bbbbbbb|Reject|
|aaaaaaabbbbbbbb|Accept|
|bbbb|Reject|
|aaaabbbbaaaa|Reject|
|bbbaaaabbbb|Accept|
|aaaaaaaabbbbbbbb|Accept|
|abababab|Accept|
|ababababab|Accept|
|aaabbbbaa|Accept|
|abbbaaaa|Accept|
|bbbaaabbb|Accept|
|(empty string)|Accept|
|a|Accept|
|b|Accept|
|aaabbb|Accept|
|abababababab|Accept|