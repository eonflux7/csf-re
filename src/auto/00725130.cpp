#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static bool UsesInheritedTextColor(const float* color)
{
    std::uint32_t bits;
    std::memcpy(&bits, color, sizeof(bits));
    return bits == 0xBF800000u;
}

static void CopyTextColorBits(float* destination, const float* source)
{
    for (int component = 0; component < 4; ++component)
    {
        std::uint32_t bits;
        std::memcpy(&bits, source + component, sizeof(bits));
        std::memcpy(destination + component, &bits, sizeof(bits));
    }
}

static void __fastcall CGuiButton_SetText(
    CGuiButton* self, void* /*edx*/, void* text)
{
    void* oldText = reinterpret_cast<void*>(self->m_pText);
    if (oldText == text)
        return;

    if (oldText)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, oldText, 0)[0])(oldText, nullptr, 1);
        self->m_pText = 0;
    }

    self->m_pText = reinterpret_cast<std::uintptr_t>(text);
    if (!text)
        return;

    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, text, 0)[0x68 / 4])(text, nullptr, 1);

    void* currentText = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, currentText, 0)[0x6C / 4])(
            currentText, nullptr, 1);

    currentText = reinterpret_cast<void*>(self->m_pText);
    const float* textColor =
        reinterpret_cast<const float*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, currentText, 0)[0x64 / 4])(
                currentText, nullptr);

    CopyTextColorBits(self->m_textColor, textColor);

    const double scaledAlpha =
        static_cast<double>(self->m_textColor[3]) *
        static_cast<double>(g_HalfDurationScale);

    std::memcpy(
        &self->field_01d8, &self->m_textColor[3], sizeof(float));
    self->field_01dc = static_cast<float>(scaledAlpha);

    CopyTextColorBits(
        self->m_idleTextColor,
        UsesInheritedTextColor(self->m_idleTextColor)
            ? self->m_textColor : self->m_idleTextColor);

    CopyTextColorBits(
        self->m_hoverTextColor,
        UsesInheritedTextColor(self->m_hoverTextColor)
            ? self->m_textColor : self->m_hoverTextColor);

    CopyTextColorBits(
        self->m_pressedTextColor,
        UsesInheritedTextColor(self->m_pressedTextColor)
            ? self->m_textColor : self->m_pressedTextColor);

    CopyTextColorBits(
        self->m_permanentTextColor,
        UsesInheritedTextColor(self->m_permanentTextColor)
            ? self->m_textColor : self->m_permanentTextColor);
}

HOOK(0x00725130, CGuiButton_SetText, "auto");
