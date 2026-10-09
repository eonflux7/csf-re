#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_4053A0, 0x004053A0, (void* self, void* edx, float* a2, float* a3, int* a4));
CSF_FUNC(uint32_t*, __fastcall, sub_40E070, 0x0040E070, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_493D50, 0x00493D50, (void* self, void* edx, float a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);   // 0.5f
CSF_GLOBAL(float, g_ScaleY_0_6, 0x0082E038);          // 0.6f
CSF_GLOBAL(float, g_ScaleXZ_0_45, 0x00833E00);        // 0.45f

// Advances the slide/interpolation of the control while it is the active scene menu context,
// unless a shrunken-volume probe at the interpolated height hits something.
static void __fastcall CPlayerCharacter_UpdateSlideProgress(CPlayerCharacter* self, void* /*edx*/, float deltaTime)
{
    if (reinterpret_cast<uint32_t>(self) != GetSceneMenuContext(g_pCurrentScene, nullptr))
        return;
    if (!CSF_AT(uint8_t, self, 0x184))
        return;

    // 0x70-byte query/result object; the interpolated height lives at +0x68
    float queryResult[28];
    sub_40E070(queryResult, nullptr);

    queryResult[26] = (CSF_AT(float, self, 0x19C) - CSF_AT(float, self, 0x190)) * CSF_AT(float, self, 0x188)
                      + CSF_AT(float, self, 0x190);

    void* shape = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(reinterpret_cast<C_0082dd78*>(self->field_004c), nullptr));
    const float* currentExtent =
        ((const float* (__fastcall*)(void*, void*))CSF_AT(void**, shape, 0)[0x30 / 4])(shape, nullptr);

    float originalExtent[3];
    originalExtent[0] = currentExtent[0];
    originalExtent[1] = currentExtent[1];
    originalExtent[2] = currentExtent[2];

    shape = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(reinterpret_cast<C_0082dd78*>(self->field_004c), nullptr));

    float vec[3];
    vec[0] = originalExtent[0] * g_ScaleXZ_0_45;
    vec[1] = originalExtent[1] * g_ScaleY_0_6;
    vec[2] = originalExtent[2] * g_ScaleXZ_0_45;
    ((void (__fastcall*)(void*, void*, float*))CSF_AT(void**, shape, 0)[0x2C / 4])(shape, nullptr, vec);

    int queryFlags = 3;
    vec[0] = 0.0f;
    vec[2] = 0.0f;
    vec[1] = queryResult[26] - originalExtent[1] * g_HalfDurationScale;

    char blocked = sub_4053A0(CSF_AT(void*, self->field_004c, 0x8), nullptr, vec, queryResult, &queryFlags);

    shape = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(reinterpret_cast<C_0082dd78*>(self->field_004c), nullptr));
    ((void (__fastcall*)(void*, void*, float*))CSF_AT(void**, shape, 0)[0x2C / 4])(shape, nullptr, originalExtent);

    if (CSF_AT(float, self, 0x190) >= CSF_AT(float, self, 0x19C) || !blocked)
    {
        int globals = GetGlobalValue_8BC1E0();
        float progress = deltaTime * CSF_AT(float, globals, 0x920) + CSF_AT(float, self, 0x188);
        CSF_AT(float, self, 0x188) = progress;
        sub_493D50(self, nullptr, progress);
    }
}

HOOK(0x00496290, CPlayerCharacter_UpdateSlideProgress, "auto");
