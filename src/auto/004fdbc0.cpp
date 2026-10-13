#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_Value_8B0568, 0x008B0568);
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int __cdecl GameBddLoader_CleanupMenuAssets()
{
    void* assets = reinterpret_cast<void*>(g_Value_8B0568);
    if (!assets)
        return 0;

    void* child = CSF_AT(void*, assets, 0xE4);
    if (child)
    {
        void* subobject = static_cast<std::uint8_t*>(child) + 4;
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, subobject, 0)[0])(subobject, nullptr, 1);
        CSF_AT(void*, reinterpret_cast<void*>(g_Value_8B0568), 0xE4) = nullptr;
        assets = reinterpret_cast<void*>(g_Value_8B0568);
    }

    child = CSF_AT(void*, assets, 0xEC);
    if (child)
    {
        void* subobject = static_cast<std::uint8_t*>(child) + 4;
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, subobject, 0)[0])(subobject, nullptr, 1);
        CSF_AT(void*, reinterpret_cast<void*>(g_Value_8B0568), 0xEC) = nullptr;
        assets = reinterpret_cast<void*>(g_Value_8B0568);
    }

    child = CSF_AT(void*, assets, 0xE8);
    if (child)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, child, 0)[0])(child, nullptr, 1);
        CSF_AT(void*, reinterpret_cast<void*>(g_Value_8B0568), 0xE8) = nullptr;
        assets = reinterpret_cast<void*>(g_Value_8B0568);
    }

    void* block = CSF_AT(void*, assets, 0xFC);
    if (block)
    {
        CoreMemory_Free(block);
        CSF_AT(void*, reinterpret_cast<void*>(g_Value_8B0568), 0xFC) = nullptr;
        assets = reinterpret_cast<void*>(g_Value_8B0568);
    }

    int result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, assets, 0)[0])(assets, nullptr, 1);
    g_Value_8B0568 = 0;
    return result;
}

HOOK(0x004FDBC0, GameBddLoader_CleanupMenuAssets, "auto");
