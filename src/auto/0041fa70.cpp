#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));

// Stores a target position, computes the normalized direction from this object's
// position toward it, resets the aim flags, then queries the owner's state (slot 12).
// If the state is not 3, it forwards to slot 35 with (3, 0, -1, 0).
static int __fastcall C_0082e6e8_SetTargetAndAimDirection(C_0082e6e8* self, void* /*edx*/, float* a2, int a3) {
    (void)a3;

    self->field_0024 = a2[0];
    CSF_AT(float, self, 0x28) = a2[1];
    CSF_AT(float, self, 0x2C) = a2[2];

    float* objectPosition = reinterpret_cast<float*>(
        GetObjectPositionVector(reinterpret_cast<void*>(self->field_0020), nullptr));

    float dirX = a2[0] - objectPosition[0];
    float dirY = a2[1] - objectPosition[1];
    float dirZ = a2[2] - objectPosition[2];

    self->field_0030 = dirX;
    CSF_AT(float, self, 0x34) = dirY;
    CSF_AT(float, self, 0x38) = dirZ;

    NormalizeVector3(&self->field_0030, nullptr);

    self->field_0044 = 0;
    self->field_003c = 2;

    void* stateObj = reinterpret_cast<void*>(self->field_0020);
    int currentState = ((int(__fastcall*)(void*, void*))CSF_AT(void**, stateObj, 0)[0x30 / 4])(stateObj, nullptr);
    if (currentState != 3) {
        return ((int(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, stateObj, 0)[0x8C / 4])(
            stateObj, nullptr, 3, 0, -1, 0);
    }
    return currentState;
}

HOOK(0x41FA70, C_0082e6e8_SetTargetAndAimDirection, "auto");
