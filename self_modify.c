#include "self_modify.h"
#include <stdio.h>
#include <windows.h> // Required for GetModuleFileNameA if used, but argv[0] is simpler.

void write_self_delete(FILE *out) {
    fprintf(out, "    char cmd[256];\n");
    // Corrected: Changed __argv[0] to argv[0] which is standard and available in main.
    fprintf(out, "    snprintf(cmd, sizeof(cmd), \"cmd.exe /C ping 127.0.0.1 -n 2 > nul & del /F /Q \\\"%%s\\\"\", argv[0]);\n");
    fprintf(out, "    system(cmd);\n");
}