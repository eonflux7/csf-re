#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(uint32_t, g_PropsRecordBase, 0x008BB478);

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, sub_6B9930, 0x006B9930, (void* self, void* edx));

static int __cdecl GfxObjectProps_ReleaseSlot(int slotOffset)
{
    uint32_t base = g_PropsRecordBase;
    uint32_t held = CSF_AT(uint32_t, reinterpret_cast<void*>(base + slotOffset), 4);
    if (held != 0)
    {
        sub_6B9930(reinterpret_cast<void*>(held), nullptr);
        Mem_Free(reinterpret_cast<void*>(held));
        base = g_PropsRecordBase;
    }
    CSF_AT(uint32_t, reinterpret_cast<void*>(base + slotOffset), 4) = 0;
    return slotOffset;
}

HOOK(0x006B9FD0, GfxObjectProps_ReleaseSlot, "auto");
