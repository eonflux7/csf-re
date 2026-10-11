#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using AlertIconSlots = uint32_t*[2];
CSF_GLOBAL(AlertIconSlots, g_AlertIconSlots, 0x008B043C);
CSF_GLOBAL(void*, g_AlertIconOwner, 0x008B0444);

static void HudAlertIcons_ReleaseHandle(C_008343a4* self, uint32_t handle)
{
    void* renderer = reinterpret_cast<void*>(self->field_000c);
    using ReleaseFunction = void(__fastcall*)(void*, void*, uint32_t);
    reinterpret_cast<ReleaseFunction>(
        CSF_AT(void**, renderer, 0)[0x88 / 4]
    )(renderer, nullptr, handle);
}

static int __fastcall C_008343a4_ReleaseAlertIconHandles(
    C_008343a4* self, void* /*edx*/)
{
    for (unsigned int i = 0; i < 2; ++i) {
        uint32_t* handleSlot = g_AlertIconSlots[i];
        uint32_t handle = *handleSlot;
        if (handle != 0) {
            HudAlertIcons_ReleaseHandle(self, handle);
            handleSlot = g_AlertIconSlots[i];
            *handleSlot = 0;
        }
    }

    uint32_t handle = CSF_AT(uint32_t, g_AlertIconOwner, 0x1C);
    if (handle != 0) {
        HudAlertIcons_ReleaseHandle(self, handle);
        void* owner = g_AlertIconOwner;
        CSF_AT(uint32_t, owner, 0x1C) = 0;
        return static_cast<int>(reinterpret_cast<uintptr_t>(owner));
    }
    return 0;
}

HOOK(0x0049FF30, C_008343a4_ReleaseAlertIconHandles, "auto");
