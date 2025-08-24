#ifndef DIR_SCAN_H
#define DIR_SCAN_H

#include <stdio.h>

void write_dir_scan(FILE *out, const char *var_find_handle, const char *var_find_data, 
                    const char *var_out_dir, const char *var_temp_buf, const char *var_loop_counter,
                    unsigned int seed, const char *func_scan_dir, const char *func_decrypt_str, 
                    const char *func_str_contains);

#endif