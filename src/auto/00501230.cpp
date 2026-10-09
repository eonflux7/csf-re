#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, SceneLoader_ResetEntrySelection, 0x0044F650, ());
CSF_FUNC(int, __cdecl, SceneLoader_IsEntryAvailable, 0x00450610, (uint32_t index));
CSF_FUNC(int, __cdecl, SceneLoader_SelectEntry, 0x00450630, (int index, int flags));
CSF_FUNC(int, __cdecl, SceneLoader_ResetEntries, 0x004506B0, ());
CSF_FUNC(char, __fastcall, SceneLoader_UpdateCurrentScene, 0x00453CB0, (void* self, void* edx, char enabled));
CSF_FUNC(void*, __cdecl, SceneLoader_AdvanceEntry, 0x004565D0, ());
CSF_FUNC(void, __fastcall, ResetInnerBuffer, 0x00501170, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall SceneLoader_LoadSceneEntries(void* self, void* /*edx*/)
{
    alignas(4) uint8_t storage[0xC8];
    auto& data = *reinterpret_cast<S_004f21d0*>(storage);

    ResetInnerBuffer(&data, nullptr);
    const int manager = GetGlobalValue_8B056C();
    void* loader = CSF_AT(void*, reinterpret_cast<void*>(manager), 0x04);
    ((void(__fastcall*)(void*, void*, S_004f21d0*, void*))
        CSF_AT(void**, loader, 0)[0x1C / 4])(
            loader, nullptr, &data, static_cast<uint8_t*>(self) + 0x98);

    if (!CSF_AT(uint8_t, self, 0xE0))
        SceneLoader_ResetEntries();

    for (uint32_t index = 0; index < CSF_AT(uint32_t, &data, 0xA4); ++index)
    {
        if (SceneLoader_IsEntryAvailable(index))
        {
            SceneLoader_SelectEntry(static_cast<int>(index), 0);
            SceneLoader_UpdateCurrentScene(g_pCurrentScene, nullptr, 1);
        }
        else
        {
            SceneLoader_AdvanceEntry();
        }
    }

    const int selectedIndex = CSF_AT(int, &data, 0xA8);
    CSF_AT(int, self, 0x0C) = selectedIndex;
    if (selectedIndex == -1)
        SceneLoader_ResetEntrySelection();
    else
        SceneLoader_SelectEntry(selectedIndex, 0);

    int result = static_cast<int>(CSF_AT(uint32_t, &data, 0xA0));
    if (CSF_AT(uint32_t, &data, 0xA0) >= 0x10u)
    {
        result = CSF_AT(int, &data, 0x8C);
        if (result)
            result = Mem_FreeDebug(CSF_AT(void*, &data, 0x8C));
    }
    CSF_AT(uint32_t, &data, 0xA0) = 15;
    CSF_AT(uint32_t, &data, 0x9C) = 0;
    CSF_AT(uint8_t, &data, 0x8C) = 0;

    if (CSF_AT(uint32_t, &data, 0x84) >= 0x10u)
    {
        result = CSF_AT(int, &data, 0x70);
        if (result)
            result = Mem_FreeDebug(CSF_AT(void*, &data, 0x70));
    }
    CSF_AT(uint32_t, &data, 0x84) = 15;
    CSF_AT(uint32_t, &data, 0x80) = 0;
    CSF_AT(uint8_t, &data, 0x70) = 0;

    if (data.field_0068 >= 0x10u)
    {
        result = CSF_AT(int, &data.field_0054, 0);
        if (result)
            result = Mem_FreeDebug(CSF_AT(void*, &data.field_0054, 0));
    }
    data.field_0068 = 15;
    data.field_0064 = 0;
    CSF_AT(uint8_t, &data.field_0054, 0) = 0;

    if (data.field_0018 >= 8u)
    {
        result = CSF_AT(int, &data.field_0004, 0);
        if (result)
            return Mem_FreeDebug(CSF_AT(void*, &data.field_0004, 0));
    }
    return result;
}

HOOK(0x00501230, SceneLoader_LoadSceneEntries, "auto");
