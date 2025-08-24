#include "junk_code.h"
#include <stdio.h>
#include <stdlib.h>

void write_junk_code(FILE *out, unsigned int seed) {
    srand(seed);
    int junk_type = rand() % 3;
    int var_suffix = rand() % 1000;
    switch (junk_type) {
        case 0: // Dummy arithmetic
            fprintf(out, "    int junk_%d = %d;\n", var_suffix, rand() % 100);
            fprintf(out, "    junk_%d = junk_%d * %d + %d;\n", 
                    var_suffix, var_suffix, rand() % 10, rand() % 50);
            break;
        case 1: // Dummy loop
            fprintf(out, "    for (int j_%d = 0; j_%d < %d; j_%d++) {\n", 
                    var_suffix, var_suffix, rand() % 5 + 1, var_suffix);
            fprintf(out, "        int k_%d = %d;\n", var_suffix, rand() % 100);
            fprintf(out, "    }\n");
            break;
        case 2: // Dummy array operations
            fprintf(out, "    int arr_%d[5] = {%d, %d, %d, %d, %d};\n", 
                    var_suffix, rand() % 10, rand() % 10, rand() % 10, rand() % 10, rand() % 10);
            fprintf(out, "    arr_%d[rand() %% 5] = arr_%d[rand() %% 5] + arr_%d[rand() %% 5];\n", 
                    var_suffix, var_suffix, var_suffix);
            break;
    }
}