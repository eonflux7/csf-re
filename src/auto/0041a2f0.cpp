#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, IndexedChildForwardValue, 0x004084E0,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(double, __stdcall, Vehicle_ComputeMotionResponse, 0x00418830,
         (float* a1, float* arg4, float* a3, float a4));
CSF_FUNC(int, __fastcall, Vehicle_GrowConstraintArray, 0x00419920,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0,
         (C_0082dd78* self, void*));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(float, g_flt_0082E25C, 0x0082E25C);

static void __fastcall C_0082e1e8_UpdateMotionAndAppendConstraint(
    C_0082e1e8* self, void* /*edx*/, int a2)
{
    auto* input = reinterpret_cast<uint8_t*>(a2);
    const int contactCount = CSF_AT(int, input, 0x04);
    if (contactCount < 1)
        return;

    const uint32_t endpointCount = self->field_003c;
    bool usePrimary = true;

    if (contactCount == 1) {
        if (!(self->field_0070 > g_fZero))
            usePrimary = false;
    } else if (CSF_AT(int, input, 0x0C) != -1 ||
               CSF_AT(int, input, 0x14) != -1) {
        if (CSF_AT(int, input, 0x08) == -1 &&
            CSF_AT(int, input, 0x10) == -1) {
            usePrimary = false;
        } else {
            if (std::fabs(self->field_0070) < g_ProgressScale) {
                self->field_0070 = 0.0f;
                return;
            }
            if (self->field_0070 > g_fZero)
                usePrimary = false;
        }
    }

    auto* endpoints = reinterpret_cast<uint32_t*>(
        usePrimary ? self->field_0034 : self->field_0044);

    float lastEndpoint[4] = {};
    float firstEndpoint[3] = {};

    IndexedChildForwardValue(
        reinterpret_cast<void*>(self->field_0030), nullptr,
        reinterpret_cast<int>(lastEndpoint),
        static_cast<int>(endpoints[8 * endpointCount - 8]));
    IndexedChildForwardValue(
        reinterpret_cast<void*>(self->field_0030), nullptr,
        reinterpret_cast<int>(firstEndpoint),
        static_cast<int>(endpoints[0]));

    void* returnedObject = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(self), nullptr));
    float* metric = ((float*(__fastcall*)(void*, void*))
        CSF_AT(void**, returnedObject, 0)[0x30 / 4])(
            returnedObject, nullptr);
    lastEndpoint[3] = *metric;

    double response = Vehicle_ComputeMotionResponse(
        lastEndpoint, firstEndpoint,
        reinterpret_cast<float*>(input + 0x38), lastEndpoint[3]);
    response *= g_flt_0082DC84;

    if (usePrimary) {
        response *= (self->field_0080 - CSF_AT(float, input, 0x34)) /
                    self->field_0080;
    } else {
        response *= (self->field_0080 - CSF_AT(float, input, 0x30)) /
                    self->field_0080;
    }

    if ((usePrimary && response < g_TransitionOne) ||
        (!usePrimary && response > g_MissingMetricValue)) {
        if (response * self->field_0070 < g_fZero)
            self->field_0070 = 0.0f;
    }

    double motion = response * CSF_AT(float, input, 0x00) +
                    self->field_0070;
    self->field_0070 = static_cast<float>(motion);

    float* resistanceMetric = reinterpret_cast<float*>(input + 0x30);
    if (CSF_AT(float, input, 0x34) < CSF_AT(float, input, 0x30))
        resistanceMetric = reinterpret_cast<float*>(input + 0x34);

    double resistance = static_cast<double>(*resistanceMetric) *
                        g_flt_0082E25C;
    if (resistance != g_fZero) {
        resistance *= CSF_AT(float, input, 0x00);
        resistance *= g_HalfDurationScale;
        if (motion > g_fZero) {
            motion -= resistance;
            self->field_0070 = static_cast<float>(motion);
            if (motion < g_fZero)
                self->field_0070 = 0.0f;
        } else {
            motion += resistance;
            self->field_0070 = static_cast<float>(motion);
            if (motion > g_fZero)
                self->field_0070 = 0.0f;
        }
    }

    float constraint[7];
    std::memcpy(constraint, lastEndpoint, sizeof(firstEndpoint));
    std::memcpy(constraint + 3, firstEndpoint, sizeof(firstEndpoint));
    constraint[6] = -(CSF_AT(float, input, 0x00) * self->field_0070);

    if (CSF_AT(uint32_t, input, 0x54) >=
        CSF_AT(uint32_t, input, 0x50)) {
        Vehicle_GrowConstraintArray(
            input + 0x4C, nullptr, CSF_AT(int, input, 0x58));
    }

    const uint32_t index = CSF_AT(uint32_t, input, 0x54);
    auto* destination = CSF_AT(uint8_t*, input, 0x4C) + 0x1C * index;
    CSF_AT(uint32_t, input, 0x54) = index + 1;
    std::memcpy(destination, constraint, 0x1C);
}

HOOK(0x0041A2F0, C_0082e1e8_UpdateMotionAndAppendConstraint, "auto");
