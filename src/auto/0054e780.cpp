#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Original: MenuMpOnline cleanup. Frees the online-menu state block at g_Global_8B0680.
CSF_GLOBAL(int, g_Global_8B0680, 0x008B0680);

CSF_FUNC(void*, __fastcall, sub_54E700, 0x0054E700, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int MenuMpOnline_Release()
{
    int result = 0;

    if (g_Global_8B0680 != 0)
    {
        uintptr_t stateAddr = static_cast<uintptr_t>(g_Global_8B0680);
        uint8_t* state = reinterpret_cast<uint8_t*>(stateAddr);

        sub_54E700(reinterpret_cast<void*>(stateAddr), nullptr);

        uint32_t ownedBlock = CSF_AT(uint32_t, state, 4);
        if (ownedBlock != 0)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(ownedBlock)));
        }

        CSF_AT(uint32_t, state, 4) = 0;
        CSF_AT(uint32_t, state, 8) = 0;
        CSF_AT(uint32_t, state, 12) = 0;

        result = Mem_Free(reinterpret_cast<void*>(stateAddr));
        g_Global_8B0680 = 0;
    }

    return result;
}

HOOK(0x0054E780, MenuMpOnline_Release, "auto");
