#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, TraceGround, 0x00405370, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(char, __fastcall, TraceGroundWithState, 0x004053A0, (void* self, void* edx, float* a2, float* a3, int* a4));
CSF_FUNC(int, __fastcall, Vector3ExactEquals, 0x004069B0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __stdcall, ReportLandingEvent, 0x0040FEE0, (int a1));
CSF_FUNC(float*, __fastcall, GetMotionVector, 0x00411EF0, (void* self, void* edx, float* a2));
CSF_FUNC(void, __fastcall, UpdateVerticalMotion, 0x00412520, (void* self, void* edx, float a2));
CSF_FUNC(uint32_t*, __fastcall, ColorAnimBufferInit, 0x00412AB0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(double, __fastcall, ComputeScalarForResolvedObject, 0x0048F220, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __cdecl, IsResolvedEntryActive, 0x00755880, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_FallingSpeedThreshold, 0x0082DFC4);
CSF_GLOBAL(float, g_LandingThresholdScale, 0x0082E050);
CSF_GLOBAL(float, g_NoFallPeak, 0x0082E054);
CSF_GLOBAL(float, g_GroundProbeSpeed, 0x0082E058);

static void __fastcall C_0082dfd0_UpdateGroundContact(
    C_0082dfd0* self, void* /*edx*/, float elapsedTime, int motionBits)
{
    // The two-float trace prefix and the following contact record form one output.
    alignas(4) uint8_t traceStorage[0x64] = {};
    uint8_t* contact = traceStorage + 8;
    CSF_AT(float, contact, 0x24) = 1.0f;
    CSF_AT(uint8_t, contact, 0x40) = 1;

    alignas(4) uint8_t position[0x10] = {};
    float motion[3] = {};
    const uint8_t wasGrounded = self->field_0088;
    char foundGround = 0;

    float motionStep;
    std::memcpy(&motionStep, &motionBits, sizeof(motionStep));
    UpdateVerticalMotion(self, nullptr, motionStep);

    bool closeToGround = false;
    if (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x50) == 0) {
        const bool useSimpleTrace = static_cast<uint8_t>(
            IsField1BCNonNegative(reinterpret_cast<void*>(self->field_0004), nullptr)) != 0;
        motion[0] = 0.0f;
        motion[1] = elapsedTime * g_GroundProbeSpeed;
        motion[2] = 0.0f;

        if (useSimpleTrace) {
            foundGround = TraceGround(
                reinterpret_cast<void*>(self->field_0008), nullptr,
                motion, reinterpret_cast<float*>(traceStorage));
        } else {
            foundGround = TraceGroundWithState(
                reinterpret_cast<void*>(self->field_0008), nullptr,
                motion, reinterpret_cast<float*>(traceStorage),
                reinterpret_cast<int*>(&self->field_0030));
        }

        if (foundGround &&
            std::fabs(CSF_AT(float, contact, 0x04) -
                      CSF_AT(float, contact, 0x54)) <= g_MessageBoxAnimationScale) {
            closeToGround = true;
        }
    }

    self->field_0088 =
        self->field_008a || (closeToGround && self->field_0084 <= g_fZero);

    auto* objectPosition = reinterpret_cast<void*>(
        GetObjectPositionVector(reinterpret_cast<void*>(self->field_0004), nullptr));
    std::memcpy(position, objectPosition, 12);

    if (!self->field_0088) {
        if (foundGround && CSF_AT(float, contact, 0x28) <= g_fZero &&
            static_cast<uint8_t>(Vector3ExactEquals(
                position, nullptr, reinterpret_cast<float*>(contact)))) {
            motion[0] = CSF_AT(float, contact, 0x24) + CSF_AT(float, position, 0);
            CSF_AT(float, position, 0) = motion[0];
            motion[1] = CSF_AT(float, position, 4) + CSF_AT(float, contact, 0x28);
            CSF_AT(float, position, 4) = motion[1];
            motion[2] = CSF_AT(float, contact, 0x2C) + CSF_AT(float, position, 8);
            CSF_AT(float, position, 8) = motion[2];

            SceneLoader_ConfigureFocusEntry(
                reinterpret_cast<void*>(self->field_0004), nullptr,
                reinterpret_cast<int*>(position));
            void* returnedObject = reinterpret_cast<void*>(
                C_0082dd78_DispatchReturnedObject(
                    reinterpret_cast<C_0082dd78*>(self), nullptr));
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, returnedObject, 0)[0x28 / 4])(returnedObject, nullptr);
        }
    }

    if (self->field_0088 && foundGround) {
        if (!self->field_008a) {
            SceneLoader_ConfigureFocusEntry(
                reinterpret_cast<void*>(self->field_0004), nullptr,
                reinterpret_cast<int*>(contact));
        }
        void* owner = reinterpret_cast<void*>(self->field_0004);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, owner, 0)[0x2C / 4])(
                owner, nullptr, CSF_AT(int, contact, 0x4C));

        void* returnedObject = reinterpret_cast<void*>(
            C_0082dd78_DispatchReturnedObject(
                reinterpret_cast<C_0082dd78*>(self), nullptr));
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, returnedObject, 0)[0x28 / 4])(returnedObject, nullptr);
    }

    GetMotionVector(self, nullptr, motion);
    const uint8_t grounded = self->field_0088;
    if (grounded) {
        if (foundGround)
            self->field_0040 = CSF_AT(uint32_t, contact, 0x48);
    } else {
        self->field_0134 =
            self->field_0134 >= motion[1] ? self->field_0134 : motion[1];
    }

    if (!wasGrounded && grounded) {
        self->field_0134 -= motion[1];

        float threshold;
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0)) {
            threshold = CSF_AT(
                float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1B0);
        } else {
            threshold = CSF_AT(
                float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x184);
        }

        if (threshold * g_LandingThresholdScale < self->field_0134) {
            float allowance;
            if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0)) {
                allowance = CSF_AT(
                    float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x918);
            } else {
                allowance = CSF_AT(
                    float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x910);
            }
            self->field_0134 -= allowance;

            if (self->field_0134 <= 0.000001) {
                ReportLandingEvent(30);
            } else {
                ReportLandingEvent(29);

                alignas(4) uint8_t colorAnimation[0x34] = {};
                ColorAnimBufferInit(colorAnimation, nullptr);
                CSF_AT(int, colorAnimation, 0x14) = -1;
                CSF_AT(int, colorAnimation, 0x24) = 5;
                std::memset(colorAnimation + 4, 255, 12);

                float damageScale;
                if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0)) {
                    damageScale = CSF_AT(
                        float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x91C);
                } else {
                    damageScale = CSF_AT(
                        float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x914);
                }
                CSF_AT(int, colorAnimation, 0x10) = static_cast<int>(
                    static_cast<int64_t>(damageScale * self->field_0134));

                void* animationReceiver = CSF_AT(
                    void*, reinterpret_cast<void*>(self->field_0004), 0x38);
                reinterpret_cast<void(__fastcall*)(void*, void*, uint8_t*)>(
                    CSF_AT(void**, animationReceiver, 0)[0x8C / 4])(
                        animationReceiver, nullptr, colorAnimation);
            }
        }
        self->field_0134 = g_NoFallPeak;
    }

    C_008342e8* resolvedObject = CSF_AT(C_008342e8*, contact, 0x44);
    self->field_008d = self->field_0088 & (resolvedObject != nullptr);
    self->field_0114 = resolvedObject != nullptr;

    if (!resolvedObject ||
        !static_cast<uint8_t>(IsResolvedEntryActive(resolvedObject->field_0050))) {
        self->field_0115 = 0;
        self->field_011c = -1;
        self->field_0120 = -1;
    } else {
        resolvedObject = CSF_AT(C_008342e8*, contact, 0x44);
        self->field_0115 = 1;
        self->field_011c = resolvedObject->field_0020;
        self->field_0120 = resolvedObject->field_0024;

        auto* resolvedPosition = reinterpret_cast<uint32_t*>(
            GetObjectPositionVector(resolvedObject, nullptr));
        self->field_0124 = resolvedPosition[0];
        CSF_AT(uint32_t, self, 0x128) = resolvedPosition[1];
        CSF_AT(uint32_t, self, 0x12C) = resolvedPosition[2];

        self->field_0130 = static_cast<float>(
            ComputeScalarForResolvedObject(
                CSF_AT(C_008342e8*, contact, 0x44), nullptr));

        auto* ownerPosition = reinterpret_cast<float*>(
            GetObjectPositionVector(reinterpret_cast<void*>(self->field_0004), nullptr));
        auto* targetPosition = reinterpret_cast<float*>(
            GetObjectPositionVector(CSF_AT(C_008342e8*, contact, 0x44), nullptr));

        motion[0] = targetPosition[0] - ownerPosition[0];
        motion[1] = targetPosition[1] - ownerPosition[1];
        motion[2] = targetPosition[2] - ownerPosition[2];
        std::memcpy(&self->field_015c, &motion[0], 4);
        std::memcpy(&CSF_AT(uint32_t, self, 0x160), &motion[1], 4);
        CSF_AT(float, self, 0x164) = motion[2];

        if (!self->field_008c &&
            CSF_AT(C_008342e8*, contact, 0x44)->field_0050 == 7) {
            self->field_0088 = 1;
        }
    }

    if (!self->field_0088 && self->field_0084 < g_FallingSpeedThreshold &&
        self->field_001c == CSF_AT(
            float, reinterpret_cast<void*>(
                GetObjectPositionVector(reinterpret_cast<void*>(self->field_0004), nullptr)),
            4)) {
        self->field_0088 = 1;
    }
}

HOOK(0x00412EE0, C_0082dfd0_UpdateGroundContact, "auto");
