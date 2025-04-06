#include <stdio.h>
#include <stdlib.h>

#include "obfuscation.h"

// for loop that allocates memory to p
// if malloc fails then the loop will exit without executing.
// if malloc succeeds then the body will execute once, then free p and set it to NULL.
// `break` statements in the body of the loop will ruin the guarantee that p is freed.
#define MALLOC_FREE(p, size) for ((p) = malloc(size); p != NULL; free(p), (p) = NULL)

int main(int argc, char *argv[]) {
    if (argc > 0)
        printf("name: %s\n", argv[0]);

    ObfuscatedType1 *obf1;
    MALLOC_FREE(obf1, sizeof_ObfuscatedType1) {
        obfuscation_ObfuscatedType1_init(obf1);
        obfuscation_ObfuscatedType1_print(obf1);
        obfuscation_ObfuscatedType1_free_internal(obf1);   
    }

    printf("\n");

    ObfuscatedType2 obf2;
    obfuscation_ObfuscatedType2_init_default(&obf2);
    obfuscation_ObfuscatedType2_print(obf2);
    obfuscation_ObfuscatedType2_free_internal(&obf2);

    printf("\n");

    obf2 = obfuscation_ObfuscatedType2_default();
    obfuscation_ObfuscatedType2_print(obf2);
    obfuscation_ObfuscatedType2_free_internal(&obf2);
   return 0;
}