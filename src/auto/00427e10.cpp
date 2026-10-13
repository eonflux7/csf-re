#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_fHudFadeStep, 0x0082E04C);
CSF_GLOBAL(float, g_ObjectiveLabelWidth, 0x0082FEC4);

static int __fastcall S_00427bf0_LayoutObjectiveLabels(
    S_00427bf0* self, void* /*edx*/)
{
    alignas(8) uint8_t positionBytes[0x8];
    alignas(8) uint8_t sizeBytes[0x8];
    alignas(8) uint8_t screenBytes[0xC];

    CSF_AT(float, screenBytes, 0x0) = 0.0f;
    CSF_AT(float, screenBytes, 0x4) = 0.0f;
    Hud_WriteNormalizedScreenSize(reinterpret_cast<float*>(screenBytes));

    CSF_AT(int32_t, sizeBytes, 0x4) = 0;
    CSF_AT(int32_t, positionBytes, 0x0) = static_cast<int32_t>(
        static_cast<int64_t>(
            CSF_AT(float, screenBytes, 0x0) * g_fHudFadeStep));

    const uint32_t initialOffset = self->field_0034 + self->field_0038;
    CSF_AT(int32_t, positionBytes, 0x4) = static_cast<int32_t>(
        static_cast<int64_t>(
            static_cast<double>(initialOffset) *
            CSF_AT(float, screenBytes, 0x4)));

    CSF_AT(int32_t, sizeBytes, 0x0) = static_cast<int32_t>(
        static_cast<int64_t>(
            CSF_AT(float, screenBytes, 0x0) * g_ObjectiveLabelWidth));

    uint32_t* labelSlot = &self->field_000c;
    int32_t result = 0;
    for (int remaining = 3; remaining; --remaining, ++labelSlot)
    {
        void* label = reinterpret_cast<void*>(*labelSlot);
        reinterpret_cast<void(__fastcall*)(void*, void*, int32_t*)>(
            CSF_AT(void**, label, 0)[0x50 / 4])(
                label, nullptr, reinterpret_cast<int32_t*>(positionBytes));

        label = reinterpret_cast<void*>(*labelSlot);
        CSF_AT(int32_t, sizeBytes, 0x4) =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, label, 0)[0xA0 / 4])(label, nullptr);

        label = reinterpret_cast<void*>(*labelSlot);
        reinterpret_cast<void(__fastcall*)(void*, void*, int32_t*)>(
            CSF_AT(void**, label, 0)[0x58 / 4])(
                label, nullptr, reinterpret_cast<int32_t*>(sizeBytes));

        // Unsigned-to-floating conversion naturally replaces the original
        // signed load plus g_UnsignedIntegerBias correction.
        const uint32_t spacing = static_cast<uint32_t>(
            static_cast<int64_t>(
                static_cast<double>(self->field_0038) *
                CSF_AT(float, screenBytes, 0x4)));
        result = static_cast<int32_t>(
            spacing + static_cast<uint32_t>(CSF_AT(int32_t, sizeBytes, 0x4)));

        CSF_AT(int32_t, positionBytes, 0x4) = static_cast<int32_t>(
            static_cast<uint32_t>(CSF_AT(int32_t, positionBytes, 0x4)) +
            static_cast<uint32_t>(result));
    }
    return result;
}

HOOK(0x00427E10, S_00427bf0_LayoutObjectiveLabels, "auto");
