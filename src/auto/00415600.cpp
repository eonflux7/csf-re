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

// Sets the target vector on this object (applying it when non-zero),
// resets the movement flags and speed, and caches the object's
// position-vector Y value into field_0134.

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

CSF_FUNC(char, __fastcall, ApplyTargetVectorAndSpeed, 0x00413DE0, (void* self, void* edx, float* a2, float a3));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

static int __fastcall C_0082dfd0_SetTargetVector(C_0082dfd0* self, void* /*edx*/, float a2, uint32_t* a3, float* a4)
{
    self->field_008c = 0;
    self->field_008a = 0;
    self->field_00d4 = 0.0f;
    self->field_0084 = 0.0f;

    if (a4[0] != g_ZeroVectorX || a4[1] != g_ZeroVectorY || a4[2] != g_ZeroVectorZ)
    {
        ApplyTargetVectorAndSpeed(self, nullptr, a4, a2);
        self->field_0024 = a3[0];
        CSF_AT(uint32_t, self, 0x28) = a3[1];
        CSF_AT(uint32_t, self, 0x2C) = a3[2];
    }

    int positionVector = GetObjectPositionVector((void*)self->field_0004, nullptr);
    int result = CSF_AT(int, (void*)positionVector, 4);
    std::memcpy(&self->field_0134, &result, sizeof(float));
    return result;
}

HOOK(0x415600, C_0082dfd0_SetTargetVector, "auto");
