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

CSF_FUNC(char, __fastcall, CScene_SetFocusEntry, 0x0044FCE0, (CScene* self, void* , int requestedEntry));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __cdecl SpawnPoints_RemoveTypedEntriesFromScene()
{
    char result = CScene_SetFocusEntry(static_cast<CScene*>(g_pCurrentScene), nullptr, 0);
    CScene* scene = static_cast<CScene*>(g_pCurrentScene);
    int entryCount = CSF_AT(int, scene, 0x18);

    for (int i = 0; i < entryCount; ++i)
    {
        uint32_t entryValue = CSF_AT(uint32_t*, scene, 0x10)[i];
        result = static_cast<char>(entryValue);
        if (entryValue == 0)
            continue;

        uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryValue));
        uint32_t entryType = static_cast<uint32_t>(CSF_AT(int, entry, 0x50));
        if (entryType > 0x20)
            continue;

        switch (entryType)
        {
        case 0:
        case 0x12:
        case 0x1F:
        case 0x20:
            result = static_cast<char>(CScene_RemoveObjectSlot(scene, nullptr,
                reinterpret_cast<int>(entry + 0x1C)));
            scene = static_cast<CScene*>(g_pCurrentScene);
            break;
        default:
            continue;
        }
    }
    return result;
}

HOOK(0x00552990, SpawnPoints_RemoveTypedEntriesFromScene, "auto");
