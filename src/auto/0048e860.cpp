#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxTransform_GetRotatedPosition, 0x00409820, (void* self, void*));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_GetSlot28ItemIfUsable, 0x00487260, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, GfxTransform_GetOtherPosition, 0x00655FF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static bool __fastcall C_00833908_IsActionEligible(
    C_00833908* self, void* /*edx*/)
{
    switch (self->field_0050)
    {
    case 1u:
    {
        void* context = reinterpret_cast<void*>(
            GetSceneMenuContext(g_pCurrentScene, nullptr));
        if (!context)
            return false;
        if (CSF_AT(uint32_t, context, 0x54) != 2)
            return false;
        if (static_cast<uint8_t>(SceneObj_IsInactive(self, nullptr)) == 0)
            return false;

        void* item = reinterpret_cast<void*>(
            CCharActionFsm_GetSlot28ItemIfUsable(
                CSF_AT(CCharActionFsm*, context, 0x44), nullptr));
        if (item &&
            static_cast<int32_t>(self->field_007c) <=
                CSF_AT(int32_t, item, 0x80))
            return false;
        return true;
    }

    case 0x13u:
    case 0x16u:
    case 0x17u:
    case 0x18u:
    case 0x20u:
        return true;

    case 0x1Au:
    {
        void* context = reinterpret_cast<void*>(
            GetSceneMenuContext(g_pCurrentScene, nullptr));
        if (!context)
            return false;

        void* object = reinterpret_cast<void*>(self->field_004c);
        void* transform =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x3c / 4])(object, nullptr);

        const float* position = reinterpret_cast<const float*>(
            GfxTransform_GetRotatedPosition(transform, nullptr));
        const float* otherPosition = reinterpret_cast<const float*>(
            GfxTransform_GetOtherPosition(transform, nullptr));

        const float centerX =
            (position[0] + otherPosition[0]) * g_HalfDurationScale;
        const float centerY =
            (position[1] + otherPosition[1]) * g_HalfDurationScale;
        const float centerZ =
            (position[2] + otherPosition[2]) * g_HalfDurationScale;

        void* contextObject =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, context, 0)[0x28 / 4])(context, nullptr);
        void* positionObject =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, contextObject, 0)[0x18 / 4])(
                    contextObject, nullptr);
        const float* referencePosition =
            ((const float*(__fastcall*)(void*, void*))
                CSF_AT(void**, positionObject, 0)[0x10 / 4])(
                    positionObject, nullptr);

        const float deltaX = centerX - referencePosition[0];
        const float deltaY = centerY - referencePosition[1];
        const float deltaZ = centerZ - referencePosition[2];

        void* settings =
            reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        return std::sqrt(deltaZ * deltaZ + deltaY * deltaY +
                         deltaX * deltaX) <
               CSF_AT(float, settings, 0x2274);
    }

    default:
        return false;
    }
}

HOOK(0x0048E860, C_00833908_IsActionEligible, "auto");
