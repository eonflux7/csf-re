#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, AddObjectReference, 0x006923B0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiStatic_CopyPropertiesToObject, 0x007181E0,
         (CGuiButton* self, void* edx, void* object));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall CGuiButton_SetPermanentQuad(
    CGuiButton* self, void* /*edx*/, void* quad)
{
    std::uint32_t result = self->m_pPermanentQuad;
    if (reinterpret_cast<void*>(result) == quad)
        return static_cast<int>(result);

    if (result != 0)
    {
        result = static_cast<std::uint32_t>(ReleaseObjectReference(
            reinterpret_cast<std::uint8_t*>(result) + 4, nullptr));
        self->m_pPermanentQuad = 0;
    }

    self->m_pPermanentQuad = reinterpret_cast<std::uint32_t>(quad);
    if (quad)
    {
        AddObjectReference(static_cast<std::uint8_t*>(quad) + 4, nullptr);
        CGuiStatic_CopyPropertiesToObject(
            self, nullptr, reinterpret_cast<void*>(self->m_pPermanentQuad));

        result = self->m_pText;
        if (result == 0)
        {
            void* rectangle =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, quad, 0)[0x6C / 4])(quad, nullptr);

            const std::uint32_t second =
                CSF_AT(volatile std::uint32_t, rectangle, 0x04);
            const std::uint32_t first =
                CSF_AT(volatile std::uint32_t, rectangle, 0x00);
            const std::uint32_t heightBits =
                CSF_AT(volatile std::uint32_t, rectangle, 0x0C);

            float height;
            std::memcpy(&height, &heightBits, sizeof(height));
            const double halfHeight =
                static_cast<double>(height) * g_HalfDurationScale;

            const std::uint32_t third =
                CSF_AT(volatile std::uint32_t, rectangle, 0x08);
            (void)first;
            (void)second;
            (void)third;

            result = heightBits;
            self->field_01dc = static_cast<float>(halfHeight);
            std::memcpy(&self->field_01d8, &heightBits, sizeof(heightBits));
        }
    }

    return static_cast<int>(result);
}

HOOK(0x00724E50, CGuiButton_SetPermanentQuad, "auto");
