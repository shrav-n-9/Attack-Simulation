#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_encrypt.h"

void write_encrypted_string(FILE *output, const char *str, const char *var_name, unsigned int seed, const char *decrypt_func) {
    unsigned char key = (unsigned char)(rand() % 256);
    fprintf(output, "    const char %s_enc[] = {", var_name);
    for (int i = 0; i < strlen(str); i++) {
        fprintf(output, "0x%02x", (unsigned char)(str[i] ^ key));
        if (i < strlen(str) - 1) fprintf(output, ", ");
    }
    fprintf(output, "};\n");
    fprintf(output, "    char %s[%d];\n", var_name, (int)strlen(str) + 1);
    fprintf(output, "    %s(%s, %s_enc, %d, %d);\n", decrypt_func, var_name, var_name, (int)strlen(str), key);
}