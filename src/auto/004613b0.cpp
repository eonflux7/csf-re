#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resolves the scene handle stored in the first argument's record and sets or clears
// flag 0x40 at +0x170 of the resolved object, based on a byte reached through the
// third argument. Returns the resolved pointer, or the raw handle value when unresolved.
static int __stdcall ApplySceneHandleFlag40(int a1, int a2, uint32_t* a3)
{
    (void)a1;
    (void)a2;

    // v3 = *(*a3 + 4): record pointed to by the first dword of the argument block.
    uint32_t handleRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(a3[0])), 4);
    int handleTag = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(handleRecord)), 12);
    int result = static_cast<int>(handleRecord + 4);

    if (handleTag >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result != 0)
        {
            void* resolved = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
            uint32_t flags = CSF_AT(uint32_t, resolved, 0x170);

            // Flag source: a3[1] -> record; record+4 -> inner record; inner record byte +1.
            uint32_t flagRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(a3[1])), 4);
            uint8_t flagByte = CSF_AT(uint8_t, reinterpret_cast<void*>(static_cast<uintptr_t>(flagRecord)), 1);

            if (flagByte != 0)
                CSF_AT(uint32_t, resolved, 0x170) = flags | 0x40u;
            else
                CSF_AT(uint32_t, resolved, 0x170) = flags & 0xFFFFFFBFu;
        }
    }
    return result;
}

HOOK(0x004613B0, ApplySceneHandleFlag40, "auto");
