#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class initializer bound by address (the original calls it first with the same two stack args).
CSF_FUNC(int, __fastcall, sub_4E2110, 0x004E2110, (void* self, void* edx, int a2, int a3));

// Constructor for C_0083e8c8: runs the base initializer, then sets the vtables and the default field values.
static uint32_t* __fastcall C_0083e8c8_Construct(C_0083e8c8* self, void* /*edx*/, int a2, int a3)
{
    sub_4E2110(self, nullptr, a2, a3);

    self->field_003c = 0xFFFFFFFFu;
    self->field_0040 = 0xFFFFFFFFu;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083E8C8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083E8A8));

    self->field_0034 = 0.0f;
    self->field_0044 = -1.0f;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004E50D0, C_0083e8c8_Construct, "auto");
