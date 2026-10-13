#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// PTank plugin offset table: the global holds a base value, and each plugin
// slot is a dword stored at (base + index).
CSF_GLOBAL(int32_t, g_PTankPluginOffset, 0x8B09EC);

static int __fastcall GfxRenderEngine_SetResourceRectangle(S_006ada10* self, void* /*edx*/, uint32_t* a2) {
    // Plugin slot index stored at +4 of this (not a declared field, so CSF_AT).
    uint32_t pluginIndex = CSF_AT(uint32_t, self, 0x4);
    uint8_t* pluginBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(g_PTankPluginOffset));

    // Object pointer for the PTank plugin: *(base + index).
    uint8_t* rectOwner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, pluginBase, pluginIndex)));

    // Copy the 4-dword rectangle into the object at +0xD8.
    CSF_AT(uint32_t, rectOwner, 0xD8) = a2[0];
    CSF_AT(uint32_t, rectOwner, 0xDC) = a2[1];
    CSF_AT(uint32_t, rectOwner, 0xE0) = a2[2];
    CSF_AT(uint32_t, rectOwner, 0xE4) = a2[3];

    // Re-read the plugin object (the original reloads it) and set flag 0x80000 at +0x40.
    uint32_t pluginObject = CSF_AT(uint32_t, pluginBase, CSF_AT(uint32_t, self, 0x4));
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(pluginObject)), 0x40) |= 0x80000u;

    return static_cast<int>(pluginObject);
}

HOOK(0x006EECB0, GfxRenderEngine_SetResourceRectangle, "auto");
