#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "var_rename.h"

const char **get_variable_names(int *count) {
    if (!count) return NULL;
    
    const char *base_names[] = {
        "var_280d", "var_36fa", "var_4e2b", "var_571c", "var_6d3e",
        "var_7f4a", "var_8b5c", "var_9c6d", "var_ad7e", "var_be8f"
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
            snprintf(buffer, sizeof(buffer), "%s_%03d", base_names[i], j);
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