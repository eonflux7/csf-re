#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_GlobalValue_8BB404, 0x008BB404);
CSF_GLOBAL(int, g_008bb400, 0x008BB400);

// Temporarily installs the current file-system dispatch value into obj+0x38, calls the
// function pointer stored at (current dispatch value)+0x3C with (obj, arg), then restores
// obj+0x38 from the default dispatch value g_008bb400 and returns the callee's result.
static int __cdecl RtFSManager_InvokeWithDefaultFileSystem(void* obj, int arg) {
    CSF_AT(int, obj, 0x38) = g_GlobalValue_8BB404;
    int result = reinterpret_cast<int(__cdecl*)(int, int)>(
        CSF_AT(int, reinterpret_cast<void*>(g_GlobalValue_8BB404), 0x3C))(
        static_cast<int>(reinterpret_cast<intptr_t>(obj)), arg);
    CSF_AT(int, obj, 0x38) = g_008bb400;
    return result;
}

HOOK(0x0069FAF0, RtFSManager_InvokeWithDefaultFileSystem, "auto");
