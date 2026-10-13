#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00860a30_ClearField000c, 0x006C9FD0, (C_00860a30* self, void* /*edx*/, int arg));
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Clears the base-class field at +0x0C, zeroes the 0x3C and 0x44 fields,
// sets the 0x34 and 0x38 scale fields to 1.0, and stores g_TransitionOne divided by
// the integer stored at (field_0018 + 0x7C) into field_0040.
static int __fastcall C_00862640_InitRateFromCount(C_00862640* self, void* /*edx*/, int a2)
{
    C_00860a30_ClearField000c(self, nullptr, a2);

    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0018);
    self->field_003c = 0.0f;
    self->field_0044 = 0;

    float count = static_cast<float>(CSF_AT(int, owner, 0x7C));
    self->field_0034 = 1.0f;
    self->field_0038 = 1.0f;
    self->field_0040 = g_TransitionOne / count;
    return 1065353216; // bit pattern of 1.0f
}

HOOK(0x006F2C30, C_00862640_InitRateFromCount, "auto");
