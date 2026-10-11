#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(int32_t, g_PTankPluginOffset, 0x8B09EC);

// Walks the entry list at self+4 (count at self+0xC). For each entry, the object at entry+8
// is asked (vtable slot 0x48, cdecl, one argument) to run its update when the tank plugin
// reached through g_PTankPluginOffset reports a positive value at +4.
static void __fastcall WaterGfx_RunTankPluginUpdates(void* self, void* /*edx*/)
{
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0xC);
    for (uint32_t i = 0; i < entryCount; ++i)
    {
        uint32_t entriesAddress = CSF_AT(uint32_t, self, 4);
        uint32_t* entries = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(entriesAddress));
        uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entries[i]));

        void* entryObject = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, entry, 8)));
        uint32_t pluginAddress = CSF_AT(uint32_t, entryObject, g_PTankPluginOffset);
        uint8_t* plugin = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(pluginAddress));

        if (CSF_AT(int32_t, plugin, 4) > 0)
        {
            void* object = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, entry, 8)));
            ((void (__cdecl*)(void*))CSF_AT(void**, object, 0)[0x48 / 4])(object);
        }
    }
}

HOOK(0x70B340, WaterGfx_RunTankPluginUpdates, "auto");
