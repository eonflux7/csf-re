#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, RotateTransformEntryZ, 0x004081A0,
         (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, DispatchEntryVirtualSlot20, 0x00408490,
         (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0,
         (C_0082dd78* self, void*));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static float FloatFromBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static int FloatArgumentBits(float value)
{
    int bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static int __fastcall C_0082e1e8_UpdateWheelTransforms(
    C_0082e1e8* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* state =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

    int result = static_cast<int>(reinterpret_cast<uintptr_t>(state));
    if (CSF_AT(int, state, 0x78) == -1082130432)
        return result;

    void* geometry = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(self), nullptr));
    float* widthPointer =
        reinterpret_cast<float* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, geometry, 0)[0x30 / 4])(geometry, nullptr);

    float curvature = FloatFromBits(self->field_0028);
    float width = *widthPointer;
    owner = reinterpret_cast<void*>(self->field_0004);
    void* directionState = CSF_AT(void*, owner, 0x30);
    uint8_t reversed = CSF_AT(uint8_t, directionState, 0x74);
    if (reversed)
        curvature = -curvature;

    float leftTravel;
    float rightTravel;
    if (curvature == g_fZero)
    {
        float x = FloatFromBits(self->field_0020);
        float y = FloatFromBits(self->field_0024);
        leftTravel = std::sqrt(x * x + y * y);
        rightTravel = leftTravel;
    }
    else
    {
        bool negativeCurvature = curvature < g_fZero;
        float magnitude = std::fabs(curvature);
        float halfWidth = width * g_HalfDurationScale;
        if (negativeCurvature)
        {
            leftTravel = (FloatFromBits(self->field_002c) - halfWidth) * magnitude;
            rightTravel = (halfWidth + FloatFromBits(self->field_002c)) * magnitude;
        }
        else
        {
            leftTravel = (halfWidth + FloatFromBits(self->field_002c)) * magnitude;
            rightTravel = (FloatFromBits(self->field_002c) - halfWidth) * magnitude;
        }
    }

    if (reversed)
    {
        leftTravel = -leftTravel;
        rightTravel = -rightTravel;
    }

    int remaining = static_cast<int>(self->field_003c);
    for (uint32_t offset = 0; remaining > 0; --remaining, offset += 32)
    {
        auto* entry = reinterpret_cast<uint8_t*>(self->field_0034) + offset;
        float angle = leftTravel / CSF_AT(float, entry, 0x04);
        int transform = CSF_AT(int, entry, 0);
        RotateTransformEntryZ(
            reinterpret_cast<void*>(self->field_0030), nullptr, transform, angle);
    }

    remaining = static_cast<int>(self->field_004c);
    for (uint32_t offset = 0; remaining > 0; --remaining, offset += 32)
    {
        auto* entry = reinterpret_cast<uint8_t*>(self->field_0044) + offset;
        float angle = rightTravel / CSF_AT(float, entry, 0x04);
        int transform = CSF_AT(int, entry, 0);
        RotateTransformEntryZ(
            reinterpret_cast<void*>(self->field_0030), nullptr, transform, angle);
    }

    result = static_cast<int>(self->field_005c);
    if (result > 0)
    {
        remaining = result;
        uint32_t offset = 0;
        do
        {
            auto* entry = reinterpret_cast<uint8_t*>(self->field_0054) + offset;

            float amount = leftTravel / CSF_AT(float, entry, 0x10);
            int transform = CSF_AT(int, entry, 0);
            DispatchEntryVirtualSlot20(
                reinterpret_cast<void*>(self->field_0030), nullptr,
                transform, 0, FloatArgumentBits(amount));

            amount = rightTravel / CSF_AT(float, entry, 0x10);
            transform = CSF_AT(int, entry, 0x04);
            DispatchEntryVirtualSlot20(
                reinterpret_cast<void*>(self->field_0030), nullptr,
                transform, 0, FloatArgumentBits(amount));

            amount = -(leftTravel / CSF_AT(float, entry, 0x14));
            transform = CSF_AT(int, entry, 0x08);
            DispatchEntryVirtualSlot20(
                reinterpret_cast<void*>(self->field_0030), nullptr,
                transform, FloatArgumentBits(amount), 0);

            amount = -(rightTravel / CSF_AT(float, entry, 0x14));
            transform = CSF_AT(int, entry, 0x0c);
            DispatchEntryVirtualSlot20(
                reinterpret_cast<void*>(self->field_0030), nullptr,
                transform, FloatArgumentBits(amount), 0);

            offset += 24;
            result = --remaining;
        } while (remaining);
    }

    return result;
}

HOOK(0x00419650, C_0082e1e8_UpdateWheelTransforms, "auto");
