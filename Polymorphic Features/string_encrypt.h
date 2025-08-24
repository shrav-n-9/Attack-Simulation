#ifndef STRING_ENCRYPT_H
#define STRING_ENCRYPT_H
#include <stdio.h>
void write_encrypted_string(FILE *output, const char *str, const char *var_name, unsigned int seed, const char *decrypt_func);
#endif