#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, TestVerticalProbe, 0x00405370, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));

static int __fastcall C_0082dfd0_IsPositionUnblocked(
    C_0082dfd0* self, void* /*edx*/, int positionAddress, int ignoredObjectId)
{
    float probe[0x64 / sizeof(float)]{};
    probe[11] = 1.0f;
    CSF_AT(char, probe, 0x48) = 1;

    const int* position = reinterpret_cast<const int*>(
        GetObjectPositionVector(reinterpret_cast<void*>(self->field_0004), nullptr));
    int savedPosition[3];
    savedPosition[0] = position[0];
    savedPosition[1] = position[1];
    savedPosition[2] = position[2];

    if (!self->field_0008)
        return 1;

    SceneLoader_ConfigureFocusEntry(
        reinterpret_cast<void*>(self->field_0004), nullptr,
        reinterpret_cast<int*>(positionAddress));

    void* dispatchedObject = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(self), nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, dispatchedObject, 0)[0x28 / 4])(
            dispatchedObject, nullptr);

    float direction[3] = {0.0f, 1.0f, 0.0f};
    char hit = TestVerticalProbe(
        reinterpret_cast<void*>(self->field_0008), nullptr, direction, probe);

    SceneLoader_ConfigureFocusEntry(
        reinterpret_cast<void*>(self->field_0004), nullptr, savedPosition);

    dispatchedObject = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(self), nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, dispatchedObject, 0)[0x28 / 4])(
            dispatchedObject, nullptr);

    if (hit && ignoredObjectId && ignoredObjectId == CSF_AT(int, probe, 0x4C))
        hit = 0;

    return hit == 0;
}

HOOK(0x00412910, C_0082dfd0_IsPositionUnblocked, "auto");
