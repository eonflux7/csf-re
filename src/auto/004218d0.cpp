#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, TaskState_SetAngleA, 0x0041E420, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, TaskState_SetAngleB, 0x0041E7E0, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, TaskState_SetValueC, 0x0041E820, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, TaskState_GetCurrentIndex, 0x00421750, (void* self, void* edx));
CSF_FUNC(double, __stdcall, TaskState_GetIndexValue, 0x00421880, (int a1));
CSF_FUNC(double, __fastcall, ComputeScalarForResolvedObject, 0x0048F220, (void* self, void* edx));

CSF_GLOBAL(float, g_DegToRad, 0x0082E898);

static bool __fastcall TaskState_UpdateIndex(S_00421750* self, void* /*edx*/)
{
    int newIndex = TaskState_GetCurrentIndex(self, nullptr);
    int oldIndex = static_cast<int>(self->field_0238);
    self->field_0240 = static_cast<float>(TaskState_GetIndexValue(oldIndex));
    if (newIndex == oldIndex)
        return false;

    void** obj = reinterpret_cast<void**>(self->field_0014);
    ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, obj, 0)[0x8C / 4])(
        obj, nullptr, newIndex, 0, -1, 0);

    float newValue = static_cast<float>(TaskState_GetIndexValue(newIndex));
    void* sub = (uint8_t*)self + 0x244;

    float angle = static_cast<float>(
        ComputeScalarForResolvedObject(reinterpret_cast<void*>(self->field_0014), nullptr) * g_DegToRad);
    TaskState_SetAngleA(sub, nullptr, angle);

    float sum = newValue + self->field_023c;
    TaskState_SetAngleB(sub, nullptr, sum);

    TaskState_SetValueC(sub, nullptr, self->field_0200);

    self->field_0238 = static_cast<uint32_t>(newIndex);
    self->field_0240 = newValue;
    return true;
}

HOOK(0x004218D0, TaskState_UpdateIndex, "auto");
