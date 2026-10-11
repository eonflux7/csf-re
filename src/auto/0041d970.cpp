#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, IndexedChild_GetForwardValue, 0x004084E0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, IndexedChild_Get, 0x004084C0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, FindOrCreateByKey, 0x004087D0, (void* self, void* edx, int a3));
CSF_FUNC(float*, __fastcall, ApplyWheelParams, 0x0040B550, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(double, __cdecl, CalculateArcCosine, 0x0040C980, (float a1));
CSF_FUNC(int, __fastcall, UpdateWheelState, 0x0041D640, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));

CSF_GLOBAL(float, g_WheelAnglePositiveLimit, 0x0082E4B4);
CSF_GLOBAL(float, g_WheelAngleNegativeLimit, 0x0082E4B8);

static char __fastcall C_0082e428_AlignWheelSteering(C_0082e428* self, void* /*edx*/)
{
    float origin[6];
    float dir[3];
    std::uint32_t matInfo[3];
    float clamped;
    float delta[4];

    void* owner = reinterpret_cast<void*>(self->field_0030);
    IndexedChild_GetForwardValue(owner, nullptr, reinterpret_cast<int>(origin), 26);

    int entry = IndexedChild_Get(reinterpret_cast<void*>(self->field_0030), nullptr, 26);
    ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, reinterpret_cast<void*>(entry), 0)[0x3C / 4])(
        reinterpret_cast<void*>(entry), nullptr, matInfo);

    dir[0] = self->field_0094 - origin[0];
    dir[1] = self->field_0098 - origin[1];
    dir[2] = self->field_009c - origin[2];
    NormalizeVector3(dir, nullptr);

    float* mat = reinterpret_cast<float*>(matInfo[0]);
    float dot = dir[1] * mat[9] + dir[0] * mat[8] + dir[2] * mat[10];
    double angle = CalculateArcCosine(dot);
    if (dir[1] > mat[9])
        angle = -angle;

    double total = angle + self->field_00a0;
    clamped = (float)total;
    if (total >= g_WheelAngleNegativeLimit) {
        if (clamped > g_WheelAnglePositiveLimit)
            clamped = 0.43633232f;
    } else {
        clamped = -0.43633232f;
    }

    void* child = FindOrCreateByKey(reinterpret_cast<void*>(self->field_0030), nullptr, 26);
    delta[0] = clamped - self->field_00a0;
    delta[1] = 0.2f;
    delta[2] = 0.0f;
    delta[3] = -1.0f;
    ApplyWheelParams(child, nullptr, 0, delta);

    UpdateWheelState(self, nullptr, 0);

    char result = CSF_AT(char, matInfo, 4);
    self->field_008c = 2;
    if (result != 0)
        result = (char)RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matInfo[0]));
    return result;
}

HOOK(0x0041D970, C_0082e428_AlignWheelSteering, "auto");
