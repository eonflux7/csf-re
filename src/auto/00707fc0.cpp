#include <cstdint>
#include "globals.h"
#include "hook.h"

// RwRasterGetPluginOffset(int) -> plugin data offset for the raster plugin slot.
CSF_FUNC(int, __cdecl, RwRasterGetPluginOffset, 0x00627430, (int a1));

// Calls vtable slot 9 (byte offset 0x24) of the object stored at (plugin offset + *a1).
// The original leaves ecx = *a1 and edx = the object's vtable when it pushes the object
// and calls the slot, so the call passes those registers through as well.
using PluginObjectSlotFn = void(__fastcall*)(uint32_t ecxValue, void* edxVtable, void* object);

static uint32_t* __cdecl GfxAnimInterpolator_InvokePluginObjectSlot9(uint32_t* a1)
{
    int pluginOffset = RwRasterGetPluginOffset(1036);

    // The original reloads ecx from [esi] after the call.
    uint32_t ecxValue = *a1;
    void* object = *reinterpret_cast<void**>(static_cast<uint32_t>(pluginOffset) + ecxValue);

    // mov edx, [eax]: the object's vtable pointer.
    void** vtable = CSF_AT(void**, object, 0);

    // push eax (object); call [edx + 0x24]
    PluginObjectSlotFn method = reinterpret_cast<PluginObjectSlotFn>(vtable[0x24 / 4]);
    method(ecxValue, vtable, object);

    return a1;
}

HOOK(0x707FC0, GfxAnimInterpolator_InvokePluginObjectSlot9, "auto");
