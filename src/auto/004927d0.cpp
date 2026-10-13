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

CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x41E0C0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x48E430, (void* self, void* edx));

// Writes the target position vector (x, y, z) into outPosition and returns it.
// If the dispatched object reports a positive value from slot 0x48, the position
// comes from its slot 0x4C (out buffer on the stack). Otherwise it is the object's
// position vector with the Y component offset by field_01D4.
static float* __fastcall C_00833b88_ComputeTargetPosition(C_00833b88* self, void* /*edx*/, float* outPosition)
{
    // 12-byte stack object passed to slot 0x4C; its +4 dword doubles as the local Y value (ebp-8).
    alignas(8) uint8_t frame[0xC];

    void* dispatched = C_0082dd78_DispatchReturnedObject(reinterpret_cast<void*>(self->field_004c), nullptr);
    void* objectPositionVector = nullptr;
    float positionY = 0.0f;

    if (((int(__fastcall*)(void*, void*))CSF_AT(void**, dispatched, 0)[18])(dispatched, nullptr) <= 0)
    {
        objectPositionVector = GetObjectPositionVector(self, nullptr);
        CSF_AT(float, frame, 4) = CSF_AT(float, objectPositionVector, 4) + CSF_AT(float, self, 0x1D4);
        positionY = CSF_AT(float, frame, 4);
    }
    else
    {
        void* dispatchedAgain = C_0082dd78_DispatchReturnedObject(reinterpret_cast<void*>(self->field_004c), nullptr);
        objectPositionVector = ((void*(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, dispatchedAgain, 0)[19])(
            dispatchedAgain, nullptr, frame, 0);
        positionY = CSF_AT(float, objectPositionVector, 4);
    }

    int positionX = CSF_AT(int, objectPositionVector, 0);
    int positionZ = CSF_AT(int, objectPositionVector, 8);

    CSF_AT(int, outPosition, 0) = positionX;
    outPosition[1] = positionY;
    CSF_AT(int, outPosition, 8) = positionZ;
    return outPosition;
}

HOOK(0x4927D0, C_00833b88_ComputeTargetPosition, "auto");
