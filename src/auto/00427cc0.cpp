#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_LayoutInset, 0x0082FEBC);
CSF_GLOBAL(float, g_ObjectiveLayoutWidth, 0x0082FEC0);

static int __fastcall S_00427bf0_UpdateObjectiveLayout(
    S_00427bf0* self, void* /*edx*/)
{
    alignas(8) uint8_t sizeBytes[0x8]{};
    alignas(8) uint8_t positionBytes[0x8]{};
    alignas(8) uint8_t screenBytes[0xC]{};

    auto* screenScale = reinterpret_cast<float*>(screenBytes);
    Hud_WriteNormalizedScreenSize(screenScale);

    void* firstText = reinterpret_cast<void*>(self->field_0010);
    void* secondText = reinterpret_cast<void*>(self->field_0014);
    void* thirdText = reinterpret_cast<void*>(self->field_000c);

    uint32_t totalHeight = static_cast<uint32_t>(
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, firstText, 0)[0xA0 / 4])(firstText, nullptr));

    totalHeight += static_cast<uint32_t>(
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, secondText, 0)[0xA0 / 4])(secondText, nullptr));

    totalHeight += static_cast<uint32_t>(
        ((int(__fastcall*)(void*, void*))
             CSF_AT(void**, thirdText, 0)[0xA0 / 4])(thirdText, nullptr));

    // Unsigned-to-floating conversion replaces the original bias adjustment.
    const uint32_t scaledSpacing = static_cast<uint32_t>(
        static_cast<int64_t>(
            static_cast<double>(self->field_0038) * screenScale[1]));

    CSF_AT(uint32_t, sizeBytes, 0) = static_cast<uint32_t>(
        static_cast<int64_t>(screenScale[0] * g_ObjectiveLayoutWidth));
    CSF_AT(uint32_t, sizeBytes, 4) = totalHeight + 4 * scaledSpacing;

    CSF_AT(uint32_t, positionBytes, 0) = static_cast<uint32_t>(
        static_cast<int64_t>(screenScale[0] * g_LayoutInset));
    CSF_AT(uint32_t, positionBytes, 4) = static_cast<uint32_t>(
        static_cast<int64_t>(
            static_cast<double>(static_cast<int32_t>(self->field_0034)) *
            screenScale[1]));

    void* panel = reinterpret_cast<void*>(self->field_0018);
    ((void(__fastcall*)(void*, void*, int*))
         CSF_AT(void**, panel, 0)[0x1C / 4])(
        panel, nullptr, reinterpret_cast<int*>(positionBytes));

    panel = reinterpret_cast<void*>(self->field_0018);
    ((void(__fastcall*)(void*, void*, int*))
         CSF_AT(void**, panel, 0)[0x24 / 4])(
        panel, nullptr, reinterpret_cast<int*>(sizeBytes));

    panel = reinterpret_cast<void*>(self->field_0018);
    return ((int(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, panel, 0)[0x2C / 4])(
        panel, nullptr, reinterpret_cast<uint8_t*>(self) + 0x48);
}

HOOK(0x00427CC0, S_00427bf0_UpdateObjectiveLayout, "auto");
