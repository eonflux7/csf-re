#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns a pointer to the object's position vector (x at +0, y at +4, z at +8).
// Takes only the implicit this in ecx and no stack arguments.
CSF_FUNC(uint8_t*, __fastcall, GetObjectPositionVector, 0x48E430, (void* self, void* edx));

// Writes the object's position into the 12-byte buffer at outPosition, with the
// Y component increased by the float stored at +0x1D4. Returns outPosition.
// __thiscall method written as __fastcall: ecx = this, edx unused, one stack arg (ret 4).
static int __fastcall C_00833b88_GetPositionWithYOffset(C_00833b88* self, void* /*edx*/, int outPosition)
{
    uint8_t* position = GetObjectPositionVector(self, nullptr);

    float positionY = CSF_AT(float, position, 4);
    uint32_t positionX = CSF_AT(uint32_t, position, 0);
    uint32_t positionZ = CSF_AT(uint32_t, position, 8);

    float yOffset = CSF_AT(float, self, 0x1D4);
    float adjustedY = positionY + yOffset;

    uint8_t* out = reinterpret_cast<uint8_t*>(outPosition);
    CSF_AT(uint32_t, out, 0) = positionX;
    CSF_AT(float, out, 4) = adjustedY;
    CSF_AT(uint32_t, out, 8) = positionZ;

    return outPosition;
}

HOOK(0x492790, C_00833b88_GetPositionWithYOffset, "auto");
