#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Allocates a 0x8C-byte, 64-aligned buffer through the original allocator.
CSF_FUNC(int, __cdecl, AllocateAlignedBuffer, 0x00690EB0,
         (std::size_t size, int alignment, int option1, int option2));

// Constructor-style initializer for C_00867318 (child object type 38).
// Sets default state, allocates a 0x8C-byte aligned buffer, and returns this.
static uint32_t* __fastcall C_00867318_InitChildObject38(C_00867318* self, void* /*edx*/, int ownerValue) {
    self->vtbl = CSF_VTBL(0x00867318);

    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0.0f;
    self->field_0028 = 0.0f;
    self->field_002c = 0.0f;
    self->field_0030 = 1.0f;

    self->field_0004 = static_cast<uint32_t>(ownerValue);
    self->field_0008 = 0;

    uint32_t alignedBuffer = static_cast<uint32_t>(AllocateAlignedBuffer(0x8C, 0x40, 0, 0));

    self->field_0024 = 255.0f;
    self->field_000c = alignedBuffer;
    self->field_0028 = 255.0f;
    self->field_0010 = 1;
    self->field_002c = 255.0f;
    self->field_0034 = 1;
    self->field_0030 = 255.0f;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00738610, C_00867318_InitChildObject38, "auto");
