# CISC/CMPE223 Assignment 1 - Problem 6
## Problem Statement
(10 marks)
Write a code to validate email addresses using a state machine approach. Implement a function that determines email validity based on state transitions, where the local part can contain letters, dots, hyphens, and underscores, and the domain part must contain letters with a dot followed by additional letters. Create a test suite with at least 5 different email inputs to demonstrate the validation logic.

## Solution
I am interpreting the problem statement to describe an email using the following regular expression: `^[a-zA-Z._-]*@[a-zA-Z]+\.[a-zA-Z]+`

I implemented a parser to parse strings that match that above patttern.

## How to run the program
Build the program:
```
gcc -o email_address_parser src/email_address_parser.c
```
Run the program:
```
./email_address_parser
```
