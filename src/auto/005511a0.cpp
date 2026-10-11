#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, SpawnPoints_GetParamRefFromSlot, 0x004A1980, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, SpawnPoints_ActivateSpawn, 0x00742360, (void* self, void* edx, char a2, char a3, char a4, char a5));

static char __stdcall SpawnPoints_TriggerFromHandles(uint32_t* a2)
{
    // Resolve the two handles stored in the request block (a2+0x08 and a2+0x14).
    int resolvedA = CHandleTable_Resolve(g_pCurrentScene, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(a2 + 2)));
    int resolvedB = CHandleTable_Resolve(g_pCurrentScene, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(a2 + 5)));

    // Without a fallthrough value, the original returns the low byte of the second resolve.
    char result = static_cast<char>(resolvedB);

    if (resolvedA && resolvedB)
    {
        uint32_t entryA = CSF_AT(uint32_t, reinterpret_cast<void*>(resolvedA), 0x18);
        C_00858180* objectA = nullptr;
        if (entryA)
            objectA = reinterpret_cast<C_00858180*>(static_cast<uintptr_t>(entryA - 120));

        uint32_t entryB = CSF_AT(uint32_t, reinterpret_cast<void*>(resolvedB), 0x18);
        C_00858180* objectB = nullptr;
        if (entryB)
            objectB = reinterpret_cast<C_00858180*>(static_cast<uintptr_t>(entryB - 1712));

        // The original overwrites its return value with entryB before the null checks.
        result = static_cast<char>(entryB);

        if (objectA && objectB)
        {
            uint32_t* paramRef = SpawnPoints_GetParamRefFromSlot(objectB, nullptr,
                reinterpret_cast<uint32_t*>(&a2));
            result = SpawnPoints_ActivateSpawn(objectA, nullptr,
                static_cast<char>(a2[13]),
                static_cast<char>(a2[14]),
                static_cast<char>(a2[15]),
                static_cast<char>(*paramRef));
        }
    }
    return result;
}

HOOK(0x5511A0, SpawnPoints_TriggerFromHandles, "auto");
