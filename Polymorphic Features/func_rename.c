#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func_rename.h"

const char **get_function_names(int *count, unsigned int seed) {
    if (!count) return NULL;
    
    srand(seed);
    const char *base_names[] = {
        "func_60c7", "func_71d8", "func_82e9", "func_93fa", "func_a50b",
        "func_b61c", "func_c72d", "func_d83e", "func_e94f", "func_fa60"
    };
    int base_count = sizeof(base_names) / sizeof(base_names[0]);
    
    *count = base_count * 10;
    const char **names = malloc(*count * sizeof(char *));
    if (!names) {
        *count = 0;
        return NULL;
    }
    
    char buffer[16];
    for (int i = 0; i < base_count; i++) {
        for (int j = 0; j < 10; j++) {
            snprintf(buffer, sizeof(buffer), "%s_%03d", base_names[i], rand() % 1000);
            names[i * 10 + j] = strdup(buffer);
            if (!names[i * 10 + j]) {
                for (int k = 0; k < i * 10 + j; k++) free((void *)names[k]);
                free(names);
                *count = 0;
                return NULL;
            }
        }
    }
    
    return names;
}