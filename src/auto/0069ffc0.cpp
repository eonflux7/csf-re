#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sets the flag at offset 0x20 of the file-system descriptor to 1 and returns 1.
static int __cdecl RtFs_SetDefaultFileSystemFlag(int descriptor) {
    CSF_AT(int, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(descriptor)), 0x20) = 1;
    return 1;
}

HOOK(0x69FFC0, RtFs_SetDefaultFileSystemFlag, "auto");
