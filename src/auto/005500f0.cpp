#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_MpCharacterSelect, 0x008B0684);
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, MpSpawnIndexObject_Construct, 0x0054FA80, (void* self, void* edx, int a2));

static bool __cdecl MpCharacterSelect_ApplySpawnIndex(int a2)
{
    if (g_MpCharacterSelect) {
        void** vtbl = CSF_AT(void**, g_MpCharacterSelect, 0);
        ((void(__fastcall*)(void*, void*, int))vtbl[0])(g_MpCharacterSelect, nullptr, 1);
        g_MpCharacterSelect = nullptr;
    }

    void* spawnBlock = Mem_Alloc(4256);
    if (spawnBlock) {
        int constructed = MpSpawnIndexObject_Construct(spawnBlock, nullptr, a2);
        g_MpCharacterSelect = reinterpret_cast<void*>(static_cast<std::uintptr_t>(constructed));
        return g_MpCharacterSelect != nullptr;
    }

    g_MpCharacterSelect = nullptr;
    return false;
}

HOOK(0x005500F0, MpCharacterSelect_ApplySpawnIndex, "auto");
