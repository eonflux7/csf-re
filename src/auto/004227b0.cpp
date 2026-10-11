#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: 0x6AB930, __fastcall with ecx = S_00526d40*, ret 0.
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

// Callee: 0x4226C0, __thiscall (ecx = this), ret 8 (float a2, char a3).
CSF_FUNC(int, __fastcall, ApplyHeadingAngle, 0x004226C0, (void* self, void* edx, float a2, char a3));

// Computes the heading from a reference point (returned by the vtable call on
// this->field_0014, slot 0xA4) to the target point a2, and applies it.
static int __fastcall C_0082f9f8_OrientTowardPoint(C_0082f9f8* self, void* /*edx*/, float* targetPoint)
{
    float referencePoint[3];

    // Vtable slot 0xA4 (slot 41) on the object at this->field_0014 fills referencePoint.
    void* referenceObject = reinterpret_cast<void*>(self->field_0014);
    ((void(__fastcall*)(void*, void*, float*))(CSF_AT(void**, referenceObject, 0)[0xA4 / 4]))(
        referenceObject, nullptr, referencePoint);

    // Build a 2D (x, z) offset vector: x = target.x - ref.x, y = 0, z = target.z - ref.z.
    S_00526d40 offsetVector;
    CSF_AT(float, &offsetVector, 0x0) = targetPoint[0] - referencePoint[0];
    CSF_AT(float, &offsetVector, 0x4) = 0.0f;
    CSF_AT(float, &offsetVector, 0x8) = targetPoint[2] - referencePoint[2];

    float headingAngle = static_cast<float>(S_00526d40_GetVectorAngleRadians(&offsetVector, nullptr));
    return ApplyHeadingAngle(self, nullptr, headingAngle, 0);
}

HOOK(0x004227B0, C_0082f9f8_OrientTowardPoint, "auto");
