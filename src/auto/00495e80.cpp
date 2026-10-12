#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));

CSF_GLOBAL(float, g_ContactClampMinimum, 0x0082E040);
CSF_GLOBAL(float, g_TimedHeightAcceleration, 0x00833DF8);
CSF_GLOBAL(float, g_TimedHeightClampOffset, 0x00833DFC);

static float DecodeStoredFloat(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static uint32_t EncodeStoredFloat(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static void __fastcall CPlayerCharacter_FinishTimedUpdate(
    CPlayerCharacter* self, void* /*edx*/, float deltaTime)
{
    if (self->field_0228)
    {
        self->field_0404 = 0;
        return;
    }

    void* position = GetObjectPositionVector(self, nullptr);
    float positionY = CSF_AT(float, position, 0x04);
    uint32_t positionX = CSF_AT(uint32_t, position, 0x00);
    uint32_t positionZ = CSF_AT(uint32_t, position, 0x08);
    void* state = reinterpret_cast<void*>(self->field_004c);
    float targetHeight = positionY + CSF_AT(float, self, 0x1D4);

    if (CSF_AT(uint8_t, state, 0x8C) ||
        CSF_AT(uint8_t, state, 0x89))
    {
        self->field_0404 = 0;
    }

    if (self->field_0404)
    {
        if (DecodeStoredFloat(self->field_0408) > targetHeight)
        {
            float nextHeight = DecodeStoredFloat(self->field_0408) -
                deltaTime * DecodeStoredFloat(self->field_040c);
            self->field_0408 = EncodeStoredFloat(nextHeight);

            if (nextHeight <= targetHeight)
            {
                self->field_0408 = EncodeStoredFloat(targetHeight);
                self->field_040c = 1116471296;
            }

            if (DecodeStoredFloat(self->field_0408) - targetHeight >
                g_ContactClampMinimum)
            {
                self->field_0408 = EncodeStoredFloat(
                    targetHeight + g_TimedHeightClampOffset);
            }
        }

        if (DecodeStoredFloat(self->field_0408) < targetHeight)
        {
            float nextHeight =
                deltaTime * DecodeStoredFloat(self->field_040c) +
                DecodeStoredFloat(self->field_0408);
            self->field_0408 = EncodeStoredFloat(nextHeight);

            if (nextHeight >= targetHeight)
            {
                self->field_0408 = EncodeStoredFloat(targetHeight);
                self->field_040c = 1116471296;
            }

            if (targetHeight - DecodeStoredFloat(self->field_0408) >
                g_ContactClampMinimum)
            {
                self->field_0408 = EncodeStoredFloat(
                    targetHeight - g_TimedHeightClampOffset);
            }
        }

        targetHeight = DecodeStoredFloat(self->field_0408);
        self->field_040c = EncodeStoredFloat(
            deltaTime * g_TimedHeightAcceleration +
            DecodeStoredFloat(self->field_040c));
    }
    else
    {
        self->field_0404 = 1;
        self->field_0408 = EncodeStoredFloat(targetHeight);
        self->field_040c = 1116471296;
    }

    CSF_AT(uint32_t, self, 0x410) = positionX;
    CSF_AT(float, self, 0x414) = targetHeight;
    CSF_AT(uint32_t, self, 0x418) = positionZ;
}

HOOK(0x00495E80, CPlayerCharacter_FinishTimedUpdate, "auto");
