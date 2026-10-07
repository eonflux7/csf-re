#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiListBoxBase_DrawHeader(
    CGuiListBoxBase* self, void* /*edx*/)
{
    if (!self->m_pHeaderText)
        return 0;

    std::uint32_t headerPosition[2];
    // The preceding push edi saves a register; this call has one argument.
    ((void(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, self, 0)[0x160 / 4])(
            self, nullptr, headerPosition);

    if (self->field_0184)
    {
        std::uint32_t backgroundGeometry[2];
        backgroundGeometry[1] = headerPosition[1];
        backgroundGeometry[0] = headerPosition[0];

        void* background = reinterpret_cast<void*>(self->field_0184);
        ((void(__fastcall*)(void*, void*, std::uint32_t*))
            CSF_AT(void**, background, 0)[0x1C / 4])(
                background, nullptr, backgroundGeometry);

        const std::uint32_t headerHeight = self->field_0174;
        const std::uint32_t* size =
            ((const std::uint32_t*(__fastcall*)(void*, void*))
                CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);
        const std::uint32_t width = size[0];

        background = reinterpret_cast<void*>(self->field_0184);
        backgroundGeometry[0] = width;
        backgroundGeometry[1] = headerHeight;
        ((void(__fastcall*)(void*, void*, std::uint32_t*))
            CSF_AT(void**, background, 0)[0x24 / 4])(
                background, nullptr, backgroundGeometry);

        background = reinterpret_cast<void*>(self->field_0184);
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, self, 0)[0x238 / 4])(
                self, nullptr, background);
    }

    const std::uint32_t headerHeight = self->field_0174;
    const std::uint32_t* size =
        ((const std::uint32_t*(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);
    const std::uint32_t width = size[0];

    void* header = reinterpret_cast<void*>(self->m_pHeaderText);
    std::uint32_t headerSize[2];
    headerSize[0] = width;
    headerSize[1] = headerHeight;
    ((void(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, header, 0)[0x50 / 4])(
            header, nullptr, headerPosition);

    header = reinterpret_cast<void*>(self->m_pHeaderText);
    ((void(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, header, 0)[0x58 / 4])(
            header, nullptr, headerSize);

    header = reinterpret_cast<void*>(self->m_pHeaderText);
    const std::uint32_t* color =
        ((const std::uint32_t*(__fastcall*)(void*, void*))
            CSF_AT(void**, header, 0)[0x64 / 4])(header, nullptr);

    std::uint32_t originalColor[4];
    originalColor[0] = color[0];
    originalColor[1] = color[1];
    originalColor[2] = color[2];
    originalColor[3] = color[3];

    float originalAlpha;
    std::memcpy(&originalAlpha, &originalColor[3], sizeof(originalAlpha));
    const double scaledAlpha =
        static_cast<double>(originalAlpha) *
        static_cast<double>(self->field_01f0);

    std::uint32_t fadedColor[4];
    fadedColor[0] = originalColor[0];
    header = reinterpret_cast<void*>(self->m_pHeaderText);
    fadedColor[1] = originalColor[1];
    const float storedAlpha = static_cast<float>(scaledAlpha);
    std::memcpy(&fadedColor[3], &storedAlpha, sizeof(storedAlpha));
    fadedColor[2] = originalColor[2];

    ((int(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, header, 0)[0x60 / 4])(
            header, nullptr, fadedColor);

    header = reinterpret_cast<void*>(self->m_pHeaderText);
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, self, 0)[0x238 / 4])(
            self, nullptr, header);

    header = reinterpret_cast<void*>(self->m_pHeaderText);
    return ((int(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, header, 0)[0x60 / 4])(
            header, nullptr, originalColor);
}

HOOK(0x00714510, CGuiListBoxBase_DrawHeader, "auto");
