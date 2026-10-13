#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Destroys a C_00863f60 instance: releases the child object in +0x08 (via its
// slot-0 virtual, with flag 1), frees the owned buffer at +0x14 when its
// capacity (+0x28) is >= 0x10, resets the members, and frees the object itself
// when bit 0 of a2 is set. Returns this.
static void* __fastcall C_00863f60_Destroy(C_00863f60* self, void* /*edx*/, char a2)
{
    using SlotZeroFn = void(__fastcall*)(void* obj, void* edx, int deleteFlag);

    self->vtbl = CSF_VTBL(0x00863F60);

    void* child = reinterpret_cast<void*>(self->field_0008);
    if (child != nullptr)
    {
        SlotZeroFn releaseChild = reinterpret_cast<SlotZeroFn>(CSF_AT(void**, child, 0)[0]);
        releaseChild(child, nullptr, 1);
        self->field_0008 = 0;
    }

    if (self->field_0028 >= 0x10 && self->field_0014 != 0)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0014));
    }

    self->field_0028 = 15;
    self->field_0024 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0014) = 0;

    if ((a2 & 1) != 0)
    {
        CoreMemory_Free(self);
    }

    return self;
}

HOOK(0x00713FC0, C_00863f60_Destroy, "auto");
