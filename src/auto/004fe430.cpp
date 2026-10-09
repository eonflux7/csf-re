#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneTable_ApplyEntry, 0x004E8E00, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

[[maybe_unused]] static const auto g_SceneTableVtbl = CSF_VTBL(0x004E8E00);

static int __cdecl SceneLoader_SyncWorldTableEntries()
{
    // Object whose vtable slot 4 produces the table index for each pass.
    uint8_t* loaderBase = reinterpret_cast<uint8_t*>(GetValue8B0554());
    void* worldObj = reinterpret_cast<void*>(CSF_AT(uint32_t, loaderBase, 8148));

    uint8_t* settingsBase = reinterpret_cast<uint8_t*>(GetValue8B0554());
    uint8_t* settings = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, settingsBase, 8156));

    using IndexGetter = int(__fastcall*)(void* self, void* edx, int arg);
    IndexGetter getIndex = reinterpret_cast<IndexGetter>(CSF_AT(void**, worldObj, 0)[4]);

    if (CSF_AT(uint8_t, settings, 44) != 0)
    {
        uint8_t* globals = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
        int primaryIndex = getIndex(worldObj, nullptr, static_cast<int>(CSF_AT(uint32_t, globals, 2308)));
        void* tableSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554()));
        SceneTable_ApplyEntry(tableSelf, nullptr, primaryIndex);
    }

    uint8_t* globalsSecond = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    int secondaryIndex = getIndex(worldObj, nullptr, static_cast<int>(CSF_AT(uint32_t, globalsSecond, 2312)));
    void* finalSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554()));
    return SceneTable_ApplyEntry(finalSelf, nullptr, secondaryIndex);
}

HOOK(0x004FE430, SceneLoader_SyncWorldTableEntries, "auto");
