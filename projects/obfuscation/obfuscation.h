#ifndef __OBFUSCATION_H__
#define __OBFUSCATION_H__

#include <stddef.h>
#include <stdint.h>

/* This type hides the internal structure of the object but does not allow for
simple stack allocation since the sizeof_ObfuscatedType1 is for some reason
not recognized by the compiler as a constant expression.
Some kind of run-time allocation is required. */
typedef struct __ObfuscatedType1 ObfuscatedType1;

extern size_t const sizeof_ObfuscatedType1;
void obfuscation_ObfuscatedType1_init(ObfuscatedType1 *obfuscated);
void obfuscation_ObfuscatedType1_print(const ObfuscatedType1 *obfuscated);
void obfuscation_ObfuscatedType1_free_internal(ObfuscatedType1 *obfuscated);

/* This type does the same as above and also allows for the user 
to stack allocate the object and get its size using sizeof(ObfuscatedType2). */
typedef struct {
    uint8_t data[24];
} ObfuscatedType2;

ObfuscatedType2 obfuscation_ObfuscatedType2_default();
void obfuscation_ObfuscatedType2_init_default(ObfuscatedType2 *obfuscated);
void obfuscation_ObfuscatedType2_free_internal(ObfuscatedType2 *obfuscated);
void obfuscation_ObfuscatedType2_print(ObfuscatedType2 obfuscated);

#endif /* __OBFUSCATION_H__ */