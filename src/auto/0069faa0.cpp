#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_GlobalValue_8BB404, 0x008BB404);
CSF_GLOBAL(int, g_008bb400, 0x008BB400);

// Target: the function-pointer table stored in g_GlobalValue_8BB404 holds a
// __cdecl service entry at +0x38. Its signature is (a1, a2, a3).
using RtFSServiceFn = int(__cdecl*)(uint32_t*, int, int);

static int __cdecl RtFSManager_InvokeDefaultFileSystem(uint32_t* a1, int a2, int a3) {
    int result;

    CSF_AT(uint32_t, a1, 0x38) = static_cast<uint32_t>(g_GlobalValue_8BB404);
    CSF_AT(uint32_t, a1, 0x08) = static_cast<uint32_t>(a2);
    CSF_AT(uint32_t, a1, 0x0C) = 0;

    uint8_t* serviceTable = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(g_GlobalValue_8BB404));
    RtFSServiceFn serviceFn = reinterpret_cast<RtFSServiceFn>(CSF_AT(uintptr_t, serviceTable, 0x38));
    result = serviceFn(a1, a2, a3);

    CSF_AT(uint32_t, a1, 0x38) = static_cast<uint32_t>(g_008bb400);
    return result;
}

HOOK(0x0069FAA0, RtFSManager_InvokeDefaultFileSystem, "auto");
