// The host CRT initializes our own C++ bindings first. Game startup is explicit.
#include <windows.h>
#include <cstdio>
#include <cstring>
#include "globals.h"
#include "runtime.h"

// db/functions.csv: _WinMain@16; parked in queue/00509250.md.
CSF_FUNC(int, __stdcall, Game_WinMain, 0x00509250, (HINSTANCE, HINSTANCE, LPSTR, int));

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR command, int show) {
    csf::initialize_data();
    const int result = csf::run_original_initializers();
    if (result) {
        std::fprintf(stderr, "csf: C initializer failed: %d\n", result);
        return result;
    }
    char dry[8]{};
    if (GetEnvironmentVariableA("CSF_DRYRUN", dry, sizeof dry) == 1 && !std::strcmp(dry, "1")) {
        std::fputs("csf: dry run completed; WinMain not called\n", stderr);
        return 0;
    }
    return Game_WinMain(instance, previous, command, show);
}
