#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, BuildContactFrame, 0x004020A0,
         (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(double, __cdecl, ClampContactComponent, 0x0040C920,
         (float* a1, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, CharActionFsm_InitializeHitInfo, 0x0040ED90,
         (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetContactProbePosition, 0x00411EF0,
         (void* self, void* edx, float* a2));
CSF_FUNC(double, __cdecl, GfxMatrix_ComputeInverseSine, 0x00411FB0,
         (float a1));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0,
         (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010,
         (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(double, g_ContactRadiansToDegrees, 0x0082DFBC);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(float, g_ContactClampMinimum, 0x0082E040);
CSF_GLOBAL(float, g_FacingAngleOffset, 0x0082E044);
CSF_GLOBAL(float, g_LevelScaleFactor, 0x0082E048);
CSF_GLOBAL(float, g_fHudFadeStep, 0x0082E04C);

static float ContactFloat(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

// The stack argument is used as a vector pointer despite the recovered
// prototype identifying its four bytes as a float.
static void __fastcall C_0082dfd0_UpdateGroundContact(
    C_0082dfd0* self, void* /*edx*/, const float* direction)
{
    if (self->field_008c ||
        CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0004), 0x228))
    {
        self->field_008a = 0;
        self->field_00d4 = 0.0f;
        return;
    }

    float probePosition[6];
    float probeStart[3];
    GetContactProbePosition(self, nullptr, probePosition);

    if (self->field_008a)
    {
        const float offsetX =
            ContactFloat(self->field_00b4) * g_fHudFadeStep;
        const float offsetY =
            ContactFloat(self->field_00b8) * g_fHudFadeStep;
        const float offsetZ =
            ContactFloat(self->field_00bc) * g_fHudFadeStep;

        probeStart[0] = probePosition[0];
        probeStart[1] = probePosition[1];
        probeStart[2] = probePosition[2];
        probePosition[0] = probeStart[0] - offsetX;
        probePosition[1] = probeStart[1] - offsetY;
        probePosition[2] = probeStart[2] - offsetZ;
    }
    else
    {
        void* boundsObject = reinterpret_cast<void*>(
            C_0082dd78_DispatchReturnedObject(
                reinterpret_cast<C_0082dd78*>(self), nullptr));

        float* dimensions =
            reinterpret_cast<float*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, boundsObject, 0)[0x30 / 4])(
                    boundsObject, nullptr);

        const float extentX = dimensions[0];
        const float extentZ = dimensions[2];
        const float halfExtent =
            (extentX < extentZ ? extentX : extentZ) * g_HalfDurationScale;

        probeStart[2] = direction[2];
        probeStart[0] = direction[0];
        probeStart[1] = 0.0f;

        if (!(probeStart[2] * probeStart[2] +
              probeStart[0] * probeStart[0] > g_fZero))
        {
            probeStart[0] = probePosition[0];
            probeStart[1] = probePosition[1];
            probeStart[2] = probePosition[2];
            probePosition[1] -= g_LevelScaleFactor;
        }
        else
        {
            NormalizeVector3(probeStart, nullptr);

            const float offsetX = probeStart[0] * halfExtent;
            const float offsetY = probeStart[1] * halfExtent;
            const float offsetZ = probeStart[2] * halfExtent;

            probeStart[0] = probePosition[0] - offsetX;
            probeStart[1] = probePosition[1] - offsetY;
            probeStart[2] = probePosition[2] - offsetZ;
            probePosition[0] += offsetX;
            probePosition[1] += offsetY;
            probePosition[2] += offsetZ;
        }
    }

    float segment[6] = {
        probeStart[0], probeStart[1], probeStart[2],
        probePosition[0], probePosition[1], probePosition[2]
    };

    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    void* collisionScene =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, sceneController, 0)[0x3C / 4])(
                sceneController, nullptr);

    void* hit = reinterpret_cast<void*(__fastcall*)(
        void*, void*, int, float*, uint32_t*, int)>(
            CSF_AT(void**, collisionScene, 0)[0x34 / 4])(
                collisionScene, nullptr, 0, segment, &self->field_0030, 3);

    if (!self->field_008a && !hit)
    {
        segment[0] = probePosition[0];
        segment[1] = probePosition[1];
        segment[2] = probePosition[2];
        segment[3] = probeStart[0];
        segment[4] = probeStart[1];
        segment[5] = probeStart[2];

        hit = reinterpret_cast<void*(__fastcall*)(
            void*, void*, int, float*, uint32_t*, int)>(
                CSF_AT(void**, collisionScene, 0)[0x34 / 4])(
                    collisionScene, nullptr, 0, segment,
                    &self->field_0030, 3);
    }

    self->field_008a = 0;
    if (!hit)
    {
        self->field_00d4 = 0.0f;
        return;
    }

    // Contiguous storage for the hit header, vector and extended hit data.
    uint32_t hitInfo[0x94 / sizeof(uint32_t)] = {};
    uint32_t contactFrame[0x2C / sizeof(uint32_t)] = {};

    CharActionFsm_InitializeHitInfo(hitInfo, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, hit, 0)[0x0C / 4])(hit, nullptr, hitInfo);

    uint32_t* previousFrame = hitInfo + 0x68 / sizeof(uint32_t);
    std::memcpy(previousFrame, &self->field_00a8, 0x2C);
    uint32_t* newFrame =
        BuildContactFrame(contactFrame, nullptr, hitInfo + 1, hitInfo + 4);
    std::memcpy(&self->field_00a8, newFrame, 0x2C);

    float maximum = g_ProgressScale;
    float minimum = g_ContactClampMinimum;
    const float verticalComponent = static_cast<float>(
        ClampContactComponent(
            reinterpret_cast<float*>(&self->field_00b8),
            reinterpret_cast<int>(&minimum),
            reinterpret_cast<int>(&maximum)));

    self->field_00d8 = static_cast<float>(
        g_FacingAngleOffset -
        GfxMatrix_ComputeInverseSine(verticalComponent) *
            g_ContactRadiansToDegrees);

    self->field_008a =
        CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()),
               2648) * g_HalfDurationScale <= self->field_00d8 &&
        self->field_00d8 <= 70.0f;

    if (!self->field_008a)
    {
        self->field_00d4 = 0.0f;
        return;
    }

    const float normalY = ContactFloat(self->field_00b8);
    const float normalZ = ContactFloat(self->field_00bc);
    const float normalX = ContactFloat(self->field_00b4);
    const float previousX = ContactFloat(previousFrame[3]);
    const float previousY = ContactFloat(previousFrame[4]);
    const float previousZ = ContactFloat(previousFrame[5]);

    self->field_0088 = 1;

    // Inferred from the final x87 condition in the supplied pseudocode.
    if (normalX * previousX + normalY * previousY +
            normalZ * previousZ > g_fZero)
    {
        self->field_00d4 = 0.0f;
    }
}

HOOK(0x00412520, C_0082dfd0_UpdateGroundContact, "auto");
