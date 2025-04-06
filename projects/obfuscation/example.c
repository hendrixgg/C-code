#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "obfuscation.h"

/* for loop that allocates memory to p
if malloc fails then the loop will exit without executing.
if malloc succeeds then the body will execute once, then free p and set it to NULL.
use a `continue` statement to break out of the loop early and free p. */
#define MALLOC_FREE_NULL(p, size) for ((p) = malloc(size); p != NULL; free(p), (p) = NULL)
/* DO NOT USE THIS VARIABLE, it is only used to break out of the MALLOC_FREE loop. */
int __b_MALLOC_FREE;
/* similar to above but uses a flag to to break out of the loop after the first iteration and does not set p to NULL. */
#define MALLOC_FREE(p, size) for ((p) = malloc(size), __b_MALLOC_FREE = 1; __b_MALLOC_FREE; free(p), __b_MALLOC_FREE = 0)


int main(int argc, char *argv[]) {
    ObfuscatedType1 *obf1;
    ObfuscatedType2 obf2;

    if (argc > 0)
        printf("name: %s\n", argv[0]);

    MALLOC_FREE(obf1, sizeof_ObfuscatedType1) {
    /* MALLOC_FREE_NULL(obf1, sizeof_ObfuscatedType1) { */
        obfuscation_ObfuscatedType1_init(obf1);
        obfuscation_ObfuscatedType1_print(obf1);
        obfuscation_ObfuscatedType1_free_internal(obf1);   
    }
    printf("\n%p\n", (void *)obf1);

    printf("\n");

    obfuscation_ObfuscatedType2_init_default(&obf2);
    obfuscation_ObfuscatedType2_print(obf2);
    obfuscation_ObfuscatedType2_free_internal(&obf2);

    printf("\n");

    memset(&obf2, 0, sizeof(ObfuscatedType2));
    obf2 = obfuscation_ObfuscatedType2_default();
    obfuscation_ObfuscatedType2_print(obf2);
    obfuscation_ObfuscatedType2_free_internal(&obf2);
   return 0;
}