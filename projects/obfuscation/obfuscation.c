
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "obfuscation.h"


struct __ObfuscatedType1 {
    double d;
    uint32_t u;
    struct __ObfuscatedType1 *o;
};

const size_t sizeof_ObfuscatedType1 = sizeof(ObfuscatedType1);

void obfuscation_ObfuscatedType1_init(ObfuscatedType1 *obfuscated) {
    assert(obfuscated != NULL);

    obfuscated->d = 0.0;
    obfuscated->u = 0;
    obfuscated->o = malloc(sizeof(ObfuscatedType1));
    obfuscated->o->d = 0.1;
    obfuscated->o->u = 1;
    obfuscated->o->o = NULL;
}

void obfuscation_ObfuscatedType1_free_internal(ObfuscatedType1 *obfuscated) {
    assert(obfuscated != NULL);

    if (obfuscated->o != NULL) {
        obfuscation_ObfuscatedType1_free_internal(obfuscated->o);
    }
    free(obfuscated->o);
    obfuscated->o = NULL;
}

void obfuscation_ObfuscatedType1_print(const ObfuscatedType1 *obfuscated) {
    if (obfuscated != NULL) {
        printf("ObfuscatedType1: d = %f, u = %u, o = {", obfuscated->d, obfuscated->u);
        obfuscation_ObfuscatedType1_print(obfuscated->o);
        printf("}");
    } else {
        printf("NULL");
    }
}


typedef struct __ObfuscatedType2 {
    double d;
    uint32_t u;
    struct __ObfuscatedType2 *o;
} _ObfuscatedType2;

ObfuscatedType2 obfuscation_ObfuscatedType2_default() {
    _ObfuscatedType2 u2;
    assert(sizeof(ObfuscatedType2) == sizeof(_ObfuscatedType2));

    u2.d = 0.0;
    u2.u = 0;
    u2.o = malloc(sizeof(_ObfuscatedType2));
    u2.o->d = 0.1;
    u2.o->u = 1;
    u2.o->o = NULL;
    return *((ObfuscatedType2 *)&u2);
}

void obfuscation_ObfuscatedType2_init_default(ObfuscatedType2 *obfuscated) {
    assert(sizeof(ObfuscatedType2) == sizeof(_ObfuscatedType2));
    assert(obfuscated != NULL);
    *obfuscated = obfuscation_ObfuscatedType2_default();
}

void obfuscation_ObfuscatedType2_free_internal(ObfuscatedType2 *obfuscated) {
    _ObfuscatedType2 *ptr;
    assert(sizeof(ObfuscatedType2) == sizeof(_ObfuscatedType2));

    ptr = (_ObfuscatedType2 *)obfuscated;
    if (ptr->o != NULL) {
        obfuscation_ObfuscatedType2_free_internal((ObfuscatedType2 *)ptr->o);
    }
    free(ptr->o);
    ptr->o = NULL;
}

void obfuscation_ObfuscatedType2_print(const ObfuscatedType2 obfuscated) {
   _ObfuscatedType2 *ptr;
    uint64_t count;
   assert(sizeof(ObfuscatedType2) == sizeof(_ObfuscatedType2));

    ptr = (_ObfuscatedType2 *)&obfuscated;
    count = 0;
    do {
        printf("ObfuscatedType2: d = %f, u = %u, o = {", ptr->d, ptr->u);
        ++count;
        ptr = ptr->o;
    } while(ptr != NULL);
    printf("NULL");
    while (count--)
        printf("}");
}
