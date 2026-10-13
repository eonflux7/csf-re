#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, sub_519480, 0x00519480, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Destroys a C_00840cf8 object: releases the flagged sub-state, frees the owned
// buffer, resets the base vtable, and frees the object itself when bit 0 of a2 is set.
static C_00840cf8* __fastcall C_00840cf8_Destroy(C_00840cf8* self, void* /*edx*/, char a2)
{
    uint8_t hasSubState = self->field_000c;
    self->vtbl = CSF_VTBL(0x00840CF8);
    if (hasSubState) {
        sub_519480(self, nullptr);
        self->field_000c = 0;
    }
    if (self->field_0014) {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }
    self->vtbl = CSF_VTBL(0x00830438);
    if ((a2 & 1) != 0) {
        CoreMemory_Free(static_cast<void*>(self));
    }
    return self;
}

HOOK(0x00519700, C_00840cf8_Destroy, "auto");
