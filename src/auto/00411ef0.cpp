#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static float* __fastcall C_0082dfd0_GetWorldOffsetPosition(
    C_0082dfd0* self, void* /*edx*/, float* output)
{
    void* returnedObject = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(self), nullptr));

    float offset[3];
    offset[0] = CSF_AT(float, returnedObject, 0x18);
    offset[1] = CSF_AT(float, returnedObject, 0x1C);
    offset[2] = CSF_AT(float, returnedObject, 0x20);

    if (g_fZero == offset[2] * offset[2]
                     + offset[1] * offset[1]
                     + offset[0] * offset[0])
    {
        float* position = reinterpret_cast<float*>(
            GetObjectPositionVector(
                reinterpret_cast<void*>(self->field_0004), nullptr));
        output[0] = position[0];
        output[1] = position[1];
        output[2] = position[2];
        return output;
    }

    returnedObject = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(self), nullptr));

    void* owner = CSF_AT(void*, returnedObject, 0x08);
    void* transform =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x18 / 4])(owner, nullptr);

    float transformedOffset[3];
    reinterpret_cast<void (__fastcall*)(void*, void*, float*, float*)>(
        CSF_AT(void**, transform, 0)[0xA4 / 4])(
            transform, nullptr, transformedOffset, offset);

    float* position = reinterpret_cast<float*>(
        GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_0004), nullptr));

    const float z = transformedOffset[2] + position[2];
    const float y = transformedOffset[1] + position[1];
    const float x = transformedOffset[0] + position[0];
    output[0] = x;
    output[1] = y;
    output[2] = z;
    return output;
}

HOOK(0x00411EF0, C_0082dfd0_GetWorldOffsetPosition, "auto");
