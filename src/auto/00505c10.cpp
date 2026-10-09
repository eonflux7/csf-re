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

// IAT slots for kernel32 imports. The original calls through [0x82d20c] and [0x82d0e0].
CSF_GLOBAL(void*, g_pfnWaitForSingleObject, 0x0082D20C);
CSF_GLOBAL(void*, g_pfnCloseHandle, 0x0082D0E0);

// __thiscall CSceneLoader::<method>(): returns the armed flag; when it was set,
// clears it, waits on the handle at +0x110 and closes that handle.
static char __fastcall CSceneLoader_WaitAndCloseHandle(CSceneLoader* self, void* /*edx*/)
{
    char result = static_cast<char>(CSF_AT(uint8_t, self, 0xB5));
    if (result)
    {
        void* waitHandle = CSF_AT(void*, self, 0x110);
        CSF_AT(uint8_t, self, 0xB5) = 0;
        reinterpret_cast<uint32_t(__stdcall*)(void*, uint32_t)>(g_pfnWaitForSingleObject)(waitHandle, 0);
        // The original reloads [this+0x110] after the wait.
        void* closeHandle = CSF_AT(void*, self, 0x110);
        return static_cast<char>(reinterpret_cast<int(__stdcall*)(void*)>(g_pfnCloseHandle)(closeHandle));
    }
    return result;
}

HOOK(0x505C10, CSceneLoader_WaitAndCloseHandle, "auto");
