#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, sub_755940, 0x00755940, (int a1));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void* AsPtr(uint32_t value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

// Looks up a handle taken from a nested object in the current scene's handle table.
// Sets the byte at out->+1 to 1 if the resolved entry passes the state check, otherwise 0.
// Returns the out pointer when the handle is negative, otherwise 1 or 0.
static int __stdcall ResolveSceneHandleAndSetEntryStateFlag(int a1, int a2, int a3)
{
    (void)a2;

    uint32_t holder = CSF_AT(uint32_t, AsPtr(static_cast<uint32_t>(a3)), 0);
    uint32_t entry = CSF_AT(uint32_t, AsPtr(holder), 4);
    int32_t handle = CSF_AT(int32_t, AsPtr(entry), 12);
    int handleAddress = static_cast<int>(entry + 4);

    if (handle < 0)
    {
        uint32_t outBase = CSF_AT(uint32_t, AsPtr(static_cast<uint32_t>(a1)), 4);
        CSF_AT(uint8_t, AsPtr(outBase), 1) = 0;
        return static_cast<int>(outBase);
    }

    int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleAddress);
    if (resolved != 0)
    {
        uint32_t entryState = CSF_AT(uint32_t, AsPtr(static_cast<uint32_t>(resolved)), 0x50);
        if (static_cast<uint8_t>(sub_755940(static_cast<int>(entryState))) != 0)
        {
            uint32_t outBase = CSF_AT(uint32_t, AsPtr(static_cast<uint32_t>(a1)), 4);
            CSF_AT(uint8_t, AsPtr(outBase), 1) = 1;
            return 1;
        }
    }

    uint32_t outBase = CSF_AT(uint32_t, AsPtr(static_cast<uint32_t>(a1)), 4);
    CSF_AT(uint8_t, AsPtr(outBase), 1) = 0;
    return 0;
}

HOOK(0x45CEC0, ResolveSceneHandleAndSetEntryStateFlag, "auto");
