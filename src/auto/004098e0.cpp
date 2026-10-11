#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(unsigned int, __fastcall, ApplySceneEntryValue, 0x006B1840, (void* self, void* edx, int a2));

// Resets the object's vtables to the base-class state, optionally releasing the
// scene entry it held, and frees the block when bit 0 of freeFlag is set.
static void* __fastcall C_0082d844_ResetSceneEntryAndFree(C_0082d844* self, void* /*edx*/, char freeFlag)
{
    uint32_t sceneEntryValue = self->field_0064;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D844);
    CSF_AT(void*, self, 0x4) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082D828));

    if (sceneEntryValue != 0)
    {
        void* sceneOwner = reinterpret_cast<void*>(GetGlobalInt());
        ApplySceneEntryValue(sceneOwner, nullptr, static_cast<int>(sceneEntryValue));
    }

    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D410);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);

    if ((freeFlag & 1) != 0)
    {
        Mem_Free(self);
    }

    return self;
}

HOOK(0x004098E0, C_0082d844_ResetSceneEntryAndFree, "auto");
