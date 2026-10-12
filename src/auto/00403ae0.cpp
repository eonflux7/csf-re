#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original Mem_Free (game heap). Callers pass the block pointer; cdecl, pops nothing.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Base-class destructor std::exception::~exception (__thiscall, ecx = this).
CSF_FUNC(void, __fastcall, sub_7AF6FB, 0x007AF6FB, (void* self, void* edx));

// C_008769d4 destructor: releases the owned buffer when its capacity is >= 16,
// resets the string state, runs the base-class destructor, and frees the object
// itself when bit 0 of a2 is set. Returns the object's vtable slot address
// (same address as the object, since the vtable pointer sits at offset 0).
// Original: __thiscall, ret 4.
static uint32_t* __fastcall C_008769d4_Destroy(C_008769d4* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x008769D4);

    // Heap buffer is only used when capacity exceeds the small-buffer size (16).
    if (self->field_0024 >= 0x10u)
        Mem_Free(reinterpret_cast<void*>(self->field_0010));

    self->field_0024 = 15;
    self->field_0020 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0010) = 0;

    sub_7AF6FB(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x403AE0, C_008769d4_Destroy, "auto");
