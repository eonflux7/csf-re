#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void, __fastcall, C_008670f0_UpdateLayoutBounds, 0x00733460, (void* self, void* edx, float* a2));
CSF_FUNC(char, __fastcall, C_008670f0_FinalizeLayout, 0x00734360, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_008670f0_CompleteLayoutPass, 0x00734B40, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_008670f0_ReadNextCharacter, 0x00735AC0, (void* self, void* edx, wchar_t* String, float* a3));
CSF_FUNC(char, __fastcall, C_008670f0_PrepareText, 0x00735C70, (void* self, void* edx, void* a2));
CSF_FUNC(wchar_t*, __fastcall, C_008670f0_InitializeLayoutCursor, 0x00736250, (void* self, void* edx, float a2));
CSF_FUNC(wchar_t*, __fastcall, C_008670f0_LayoutNextCharacter, 0x00736800, (void* self, void* edx, uint32_t* a2, float* a3, uint32_t* a4));

CSF_GLOBAL(std::uint32_t, g_LayoutState, 0x008A88C4);
CSF_GLOBAL(std::uint8_t, g_LayoutFlag, 0x008A88CC);
CSF_GLOBAL(std::uint32_t, g_LayoutCounter, 0x008A88D0);
CSF_GLOBAL(float, g_LayoutPreviousPosition, 0x008A88D4);
CSF_GLOBAL(float, g_LayoutPosition, 0x008A88D8);

static char __fastcall C_008670f0_RebuildTextLayout(
    C_008670f0* self, void* /*edx*/)
{
    self->vSlot34();

    char result = static_cast<char>(self->field_0010);
    if (!result)
        return result;

    const bool hasText = self->field_0014 != 0;
    self->field_0010 = 0;
    self->field_0080 = 0;
    self->field_0060 = 0;
    self->field_0090 = 0;
    self->field_0070 = 0;
    self->field_00c8 = 0;

    if (!hasText)
        return result;

    auto* objectBytes = reinterpret_cast<std::uint8_t*>(self);
    AssignStringRange_488830(
        objectBytes + 0x3C, nullptr, objectBytes + 0x20, 0, 0xFFFFFFFF);
    C_008670f0_PrepareText(self, nullptr, objectBytes + 0x3C);

    alignas(8) std::uint8_t boundsBytes[0x8];
    alignas(8) std::uint8_t characterMetricBytes[0x4];
    alignas(8) std::uint8_t cursorBytes[0x4];

    float* bounds = reinterpret_cast<float*>(boundsBytes);
    bounds[0] = 0.0f;
    bounds[1] = 0.0f;
    CSF_AT(std::uint32_t, cursorBytes, 0) = 0;
    CSF_AT(std::uint32_t, characterMetricBytes, 0) = 0;

    g_LayoutState = 0;
    g_LayoutFlag = 0;
    g_LayoutCounter = 0;
    g_LayoutPreviousPosition = -1.0f;
    g_LayoutPosition = 0.0f;

    C_008670f0_InitializeLayoutCursor(self, nullptr, 0.0f);

    if (self->field_0050)
    {
        do
        {
            C_008670f0_ReadNextCharacter(
                self, nullptr,
                reinterpret_cast<wchar_t*>(cursorBytes),
                reinterpret_cast<float*>(characterMetricBytes));
            C_008670f0_LayoutNextCharacter(
                self, nullptr,
                reinterpret_cast<std::uint32_t*>(cursorBytes),
                bounds,
                reinterpret_cast<std::uint32_t*>(characterMetricBytes));
        }
        while (CSF_AT(std::uint32_t, cursorBytes, 0) < self->field_0050);
    }

    C_008670f0_CompleteLayoutPass(self, nullptr);
    C_008670f0_UpdateLayoutBounds(self, nullptr, bounds);
    result = C_008670f0_FinalizeLayout(self, nullptr);
    self->field_00d8 = 0;
    return result;
}

HOOK(0x00736D50, C_008670f0_RebuildTextLayout, "auto");
