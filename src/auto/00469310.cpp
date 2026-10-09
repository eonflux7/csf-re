#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, SceneSlotMgr_DetachEntry, 0x004690A0, (void* self, void* edx, int a2));

static void __fastcall C_00833b88_ReleaseSlot(C_00833b88* self, void* /*edx*/, void* entry)
{
    uint32_t count = self->field_0030;
    uint32_t index = 0;
    if (count == 0)
        return;

    uint32_t* slot = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_0028));
    uint32_t entryBits = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(entry));

    for (;;)
    {
        if (*slot && *slot == entryBits)
            break;
        ++index;
        ++slot;
        if (index >= count)
            return;
    }

    SceneSlotMgr_DetachEntry(self, nullptr, static_cast<int>(entryBits));

    if (entry)
    {
        void** vtbl = CSF_AT(void**, entry, 0);
        ((void(__fastcall*)(void*, void*, int))vtbl[0])(entry, nullptr, 1);
    }

    uint32_t* reloaded = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_0028));
    reloaded[index] = 0;
}

HOOK(0x00469310, C_00833b88_ReleaseSlot, "auto");
