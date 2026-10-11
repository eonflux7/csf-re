#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// CRT _beginthread: uintptr_t _beginthread(void (__cdecl*)(void*), unsigned stack_size, void* arg)
CSF_FUNC(uintptr_t, __cdecl, Crt_BeginThread, 0x007B07F5,
         (void (__cdecl* startAddress)(void*), unsigned stackSize, void* arg));

CSF_GLOBAL(uint8_t, byte_8BC120, 0x008BC120);
CSF_GLOBAL(uint8_t, byte_8A8924, 0x008A8924);

// IAT slot holding the function pointer for SetThreadPriority (stdcall, 2 args).
CSF_GLOBAL(uint32_t, g_SetThreadPriorityIat, 0x0082D208);

static int __fastcall MpSpawnPoints_ResetCharacterMenu(void* self, void* /*edx*/)
{
    int result = 0;

    if (CSF_AT(int, self, 0x98) == 6)
    {
        byte_8BC120 = 0;
        byte_8A8924 = 0;
        Crt_BeginThread(reinterpret_cast<void (__cdecl*)(void*)>(CSF_ADDR_FN(0x0073F000)),
                        0, self);
        return reinterpret_cast<int (__stdcall*)(void (__cdecl*)(void*), int)>(
            g_SetThreadPriorityIat)(
            reinterpret_cast<void (__cdecl*)(void*)>(CSF_ADDR_FN(0x0073F000)), 2);
    }
    return result;
}

HOOK(0x0073F340, MpSpawnPoints_ResetCharacterMenu, "auto");
