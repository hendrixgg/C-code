
# First few failed attempts
```C
// Write the following quoted text twice, the second time in quotes. "Write the following quoted text twice, the second time in quotes."
// Print each character after the '.' skipping over the '"' characters, then print each character after the '.' including the '"' characters."Print each character after the '.' skipping over the '\"' characters, then print each character after the '.' including the '\"' characters."

/*
#include <stdio.h>
void printf_then_printf_in_quotes(const char *s)
{
    printf("%s", s);
    printf("\"%s\"", s);
}

int main(void)
{
    printf_then_printf_in_quotes(
        "#include <stdio.h>\n"
        "void printf_then_printf_in_quotes(const char *s)\n"
        "{\n"
        "    printf(\"%s\", s);\n"
        "    printf(\"\\\"%s\\\"\", s);\n"
        "}\n"
        "int main(void)\n"
        "{\n"
        "    printf_then_printf_in_quotes(\n");
    return 0;
}
*/
/*
// B is a function that takes the input and returns the string of a program that returns the input.
#include <stdio.h>

const char *B(const char *input) { printf("void main(void) {printf(\"%%s\", \"%s\")}", input); }

// A is a function that just returns a const char * of the function B.
const char *A(void) { return "void B(const char *input) { printf(\"#include<stdio.h>\\nvoid main(void) {printf(\\\"%%s\\\", \\\"%s\\\")}\", input); }"; }

void main(void)
{
    B(A());
}
*/

// p is a function that takes the input string w, and returns the string of a function that ignores its argument and returns w. The caller is expected to free the memory of the returned string.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

const char *p(const char *w)
{
    const size_t output_length = strlen("const char *P_w(const char *s){ return \"%s\";}") + strlen(w);
    char *output = malloc(output_length + 1);
    sprintf(output, "const char *P_w(const char *s){ return \"%s\";}", w);
    assert(strlen(output) == output_length);
    return output;
}

void main(void)
{
    printf("%s\n", p("this string will always be returned"));
}

// need a function, that takes the input, and returns two copies of the input, one nested within the other.
```

# Got it basically working with but not automated

replication.c and escape.h and escape.c

# Got it working automated

template.c and Makefile