#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, sub_4954A0, 0x004954A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_497080, 0x00497080, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, sub_4EF6E0, 0x004EF6E0, (void* self, void* edx, void* a2, char a3));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneEntry, 0x00770690, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);

// Periodically re-evaluates which water/scene volume the object is in and
// updates the "submerged" state (flag 0x10000) accordingly.
static void __fastcall C_00833ee8_UpdateVolumeImmersion(C_00833ee8* self, void* /*edx*/)
{
    if ((self->field_02e0 & 0x10000) != 0)
    {
        float positionBuffer[4];
        float* position = ((float*(__fastcall*)(void*, void*, void*))CSF_AT(void**, self, 0)[0xA4 / 4])(
            self, nullptr, positionBuffer);

        // The original keeps these intermediates at full FPU precision (never stored as float).
        double height = position[1];
        if (self->field_0228)
            height = height - (double)self->field_0234;

        double depth = (double)self->field_03d4 - height + (double)g_fReturnedConstant_0082E030;
        if (depth > (double)g_fZero)
        {
            if (depth > (double)g_fReturnedConstant_0082E030)
                sub_497080(self, nullptr, 1);
            else
                sub_497080(self, nullptr, 0);
        }
        else
        {
            sub_497080(self, nullptr, 0);
        }
    }

    if (self->field_021c < g_flt_0082DC84)
        return;

    uint8_t deepFlag = self->field_03d0;
    self->field_021c = 0.0f;

    void* volumeManager = CSF_AT(void*, GetValue8B0554(), 0x1FDC);
    void* objectPosition = (void*)GetObjectPositionVector(self, nullptr);
    int volumeIndex = sub_4EF6E0(volumeManager, nullptr, objectPosition, 1);

    if (volumeIndex == -1)
    {
        if (self->field_03cc != (uint32_t)-1)
        {
            self->field_03cc = (uint32_t)-1;
            uint32_t flags = self->field_02e4;
            self->field_03d4 = 0.0f;
            deepFlag = 0;
            self->field_02e4 = flags & 0xFFFEFFFF;
        }
    }
    else
    {
        int sceneEntry = SceneLoader_GetSceneEntry(volumeManager, nullptr, volumeIndex);
        if (self->field_03cc != (uint32_t)volumeIndex)
        {
            self->field_03cc = (uint32_t)volumeIndex;
            self->field_03d4 = CSF_AT(float, sceneEntry, 0x54) + CSF_AT(float, sceneEntry, 0x50);
        }

        double immersion = sub_4954A0(self, nullptr);
        if (immersion > (double)g_fZero)
            self->field_02e4 = self->field_02e4 | 0x10000;
        else
            self->field_02e4 = self->field_02e4 & 0xFFFEFFFF;
    }

    sub_497080(self, nullptr, deepFlag);
}

HOOK(0x004977E0, C_00833ee8_UpdateVolumeImmersion, "auto");
