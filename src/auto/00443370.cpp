#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, sub_442F40, 0x00442F40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys a C_00831074 object: releases the flagged sub-state, frees the owned
// buffer, resets the base vtable, and frees the object itself when bit 0 of a2 is set.
static C_00831074* __fastcall C_00831074_DestroyObject(C_00831074* self, void* /*edx*/, char a2)
{
    uint8_t hasSubState = static_cast<uint8_t>(self->field_0004);
    self->vtbl = CSF_VTBL(0x00831074);
    if (hasSubState) {
        sub_442F40(self, nullptr);
        self->field_0004 &= ~0xFFu;  // LOBYTE(field_0004) = 0
    }
    if (self->field_000c) {
        Mem_Free(reinterpret_cast<void*>(self->field_000c));
        self->field_000c = 0;
    }
    self->vtbl = CSF_VTBL(0x0082D410);
    if ((a2 & 1) != 0) {
        Mem_Free(static_cast<void*>(self));
    }
    return self;
}

HOOK(0x00443370, C_00831074_DestroyObject, "auto");
