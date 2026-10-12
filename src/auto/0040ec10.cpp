#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_PositionHeightOffset, 0x0082DDE8);
CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);
CSF_GLOBAL(float, g_DistanceFadeScale, 0x0082DDF0);
CSF_GLOBAL(float, g_DistanceFadeLimit, 0x0082DDF4);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);

static int __fastcall C_0082ddf8_UpdateAttachedEffect(
    C_0082ddf8* self, void* /*edx*/)
{
    float fade;
    if (self->field_0054 > g_DistanceFadeLimit)
        fade = g_fZero;
    else
        fade = g_TransitionOne - self->field_0054 * g_DistanceFadeScale;

    void* effect = reinterpret_cast<void*>(self->field_0024);
    const float intensity = fade * g_Const200_0f;
    reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, effect, 0)[0x4 / 4])(
            effect, nullptr, intensity);

    alignas(8) uint8_t positionBytes[0xC];
    alignas(8) uint8_t directionBytes[0xC];

    const float height = self->field_005c + g_PositionHeightOffset;
    const uint32_t positionX = self->field_0058;
    const uint32_t positionZ = self->field_0060;
    CSF_AT(uint32_t, positionBytes, 0x0) = positionX;
    effect = reinterpret_cast<void*>(self->field_0024);
    CSF_AT(float, positionBytes, 0x4) = height;
    CSF_AT(uint32_t, positionBytes, 0x8) = positionZ;

    reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, effect, 0)[0xC / 4])(
            effect, nullptr, 250.0f);

    const float directionX = -g_AxisY;
    effect = reinterpret_cast<void*>(self->field_0024);
    CSF_AT(float, directionBytes, 0x0) = directionX;
    CSF_AT(float, directionBytes, 0x4) = -g_AxisY_Y;
    CSF_AT(float, directionBytes, 0x8) = -g_AxisY_Z;

    return reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, float*)>(
        CSF_AT(void**, effect, 0)[0x14 / 4])(
            effect, nullptr,
            reinterpret_cast<uint32_t*>(positionBytes),
            reinterpret_cast<float*>(directionBytes));
}

HOOK(0x0040EC10, C_0082ddf8_UpdateAttachedEffect, "auto");
