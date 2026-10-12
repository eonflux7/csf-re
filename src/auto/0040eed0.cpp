#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constant 300.0f used as the reference height for the offset (float at 0x0082DDE0).
CSF_GLOBAL(float, g_HeightReferenceOffset, 0x0082DDE0);

// Applies the height-based offset to the sub-object at +0x28 and forwards the
// computed position to its slot 17 method. The unknown-type sub-object is
// reached through its vtable (slot 11 = +0x2C, slot 17 = +0x44).
static char __fastcall C_0082ddf8_ApplyHeightOffsetPosition(C_0082ddf8* self, void* /*edx*/)
{
    void* subObject = reinterpret_cast<void*>(self->field_0028);
    using SetFlagFn = void(__fastcall*)(void*, void*, uint32_t);
    ((SetFlagFn)CSF_AT(void**, subObject, 0)[11])(subObject, nullptr,
        static_cast<uint32_t>(self->field_0050));

    char result = static_cast<char>(self->field_0050);
    if (result)
    {
        // Frame buffer of 0x18 bytes: [0..2] = position, [4..5] = scaled temporaries.
        float positionBuf[6];

        float heightOffset = self->field_0054 - g_HeightReferenceOffset;
        float scaledX = heightOffset * self->field_0044;
        positionBuf[4] = heightOffset * self->field_0048;
        positionBuf[5] = heightOffset * self->field_004c;
        positionBuf[0] = scaledX + self->field_002c;
        positionBuf[1] = positionBuf[4] + self->field_0030;
        positionBuf[2] = positionBuf[5] + self->field_0034;

        // Re-read the sub-object pointer as the original does after the first call.
        void* target = reinterpret_cast<void*>(self->field_0028);
        using PositionFn = char(__fastcall*)(void*, void*, float*);
        return ((PositionFn)CSF_AT(void**, target, 0)[17])(target, nullptr, positionBuf);
    }
    return result;
}

HOOK(0x0040EED0, C_0082ddf8_ApplyHeightOffsetPosition, "auto");
