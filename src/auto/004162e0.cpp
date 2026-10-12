#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, TraceGroundWithState, 0x004053A0, (void* self, void* edx, float* a2, float* a3, int* a4));
CSF_FUNC(void*, __fastcall, SpatialQuery_InitializeQueryInfo, 0x0040E070, (void* self, void*));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));

CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_Const20, 0x0082E0F0);

static int __fastcall C_0082e090_UpdateGroundAdjustedFocus(
    C_0082e090* self, void* /*edx*/,
    float* position, float* direction, int traceState)
{
    self->field_0077 = 0;

    alignas(8) uint8_t focusBytes[0xC];
    float* focus = reinterpret_cast<float*>(focusBytes);

    if (static_cast<uint8_t>(traceState) == 0)
    {
        focus[0] = position[0] + direction[0];
        focus[1] = position[1] + direction[1];
        focus[2] = position[2] + direction[2];
        SceneLoader_ConfigureFocusEntry(
            reinterpret_cast<void*>(self->field_0004), nullptr,
            reinterpret_cast<int*>(focus));

        void* object = C_0082dd78_DispatchReturnedObject(self, nullptr);
        return (reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[10]))(object, nullptr);
    }

    void* focusEntry = reinterpret_cast<void*>(self->field_0004);
    focus[0] = direction[0] * g_MessageBoxAnimationScale + position[0];
    focus[1] = direction[1] * g_MessageBoxAnimationScale + position[1];
    focus[2] = direction[2] * g_MessageBoxAnimationScale + position[2];
    SceneLoader_ConfigureFocusEntry(
        focusEntry, nullptr, reinterpret_cast<int*>(focus));

    void* object = C_0082dd78_DispatchReturnedObject(self, nullptr);
    (reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[10]))(object, nullptr);

    alignas(8) uint8_t queryBytes[0x70];
    SpatialQuery_InitializeQueryInfo(queryBytes, nullptr);

    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x5) = 1;
    focus[0] = -(direction[0] * g_Const20);
    focus[1] = -(direction[1] * g_Const20);
    focus[2] = -(direction[2] * g_Const20);
    traceState = 2;

    char hit = TraceGroundWithState(
        reinterpret_cast<void*>(self->field_0008), nullptr, focus,
        reinterpret_cast<float*>(queryBytes), &traceState);

    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x5) = 0;
    focusEntry = reinterpret_cast<void*>(self->field_0004);

    if (hit)
    {
        focus[0] = CSF_AT(float, queryBytes, 0x2C)
                 + CSF_AT(float, queryBytes, 0x08);
        focus[1] = CSF_AT(float, queryBytes, 0x30)
                 + CSF_AT(float, queryBytes, 0x0C);
        focus[2] = CSF_AT(float, queryBytes, 0x34)
                 + CSF_AT(float, queryBytes, 0x10);
    }
    else
    {
        focus[0] = position[0] + direction[0];
        focus[1] = direction[1] + position[1];
        focus[2] = direction[2] + position[2];
    }

    SceneLoader_ConfigureFocusEntry(
        focusEntry, nullptr, reinterpret_cast<int*>(focus));
    object = C_0082dd78_DispatchReturnedObject(self, nullptr);
    return (reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[10]))(object, nullptr);
}

HOOK(0x004162E0, C_0082e090_UpdateGroundAdjustedFocus, "auto");
