#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, NetCogibleDDL_GetWrappedAngleDegrees, 0x00740AA0, (void* self, void* edx));

static int __fastcall C_0083ba40_SendWrappedAngleVector(C_0083ba40* self, void* /*edx*/, CMenuMpStats* a2)
{
    void* angleSource = reinterpret_cast<void*>(self->field_0014);
    void* sourceProxy = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, angleSource, 0)[10])(angleSource, nullptr);
    void* targetObj = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, sourceProxy, 0)[6])(sourceProxy, nullptr);

    float angleVector[2];
    angleVector[0] = (float)NetCogibleDDL_GetWrappedAngleDegrees(a2, nullptr);
    angleVector[1] = 0.0f;

    return ((int(__fastcall*)(void*, void*, float*))CSF_AT(void**, targetObj, 0)[20])(targetObj, nullptr, angleVector);
}

HOOK(0x004D1D50, C_0083ba40_SendWrappedAngleVector, "auto");
