#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_SetPosition_7198A0, 0x007198A0,
         (CGuiStatic* self, void* edx, float scaleX, float scaleY));
CSF_FUNC(void, __fastcall, CGuiStatic_CopyPropertiesToObject, 0x007181E0,
         (CGuiStatic* self, void* edx, int* target));
CSF_FUNC(void, __fastcall, CGuiStatic_CopyStateToTarget, 0x00718230,
         (CGuiStatic* self, void* edx, int* target));

static int __fastcall CGuiStatic_SetScale(
    CGuiStatic* self, void* /*edx*/, float scaleX, float scaleY)
{
    CGuiControl_SetPosition_7198A0(self, nullptr, scaleX, scaleY);
    CGuiStatic_CopyPropertiesToObject(
        self, nullptr, reinterpret_cast<int*>(self->field_0138));

    float scale[2];
    if (self->m_pText != 0) {
        CGuiStatic_CopyStateToTarget(
            self, nullptr, reinterpret_cast<int*>(self->m_pText));

        scale[0] = scaleX;
        scale[1] = scaleY;

        void* text = reinterpret_cast<void*>(self->m_pText);
        ((void(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, text, 0)[0x98 / 4])(text, nullptr, scale);
    }

    void* quad = reinterpret_cast<void*>(self->m_pQuad);
    if (quad == nullptr)
        return 0;

    const void* coordinates =
        ((const void*(__fastcall*)(void*, void*))
            CSF_AT(void**, quad, 0)[0x20 / 4])(quad, nullptr);
    const std::uint32_t firstY = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(CSF_AT(std::int32_t, coordinates, 4)) * scaleY));

    quad = reinterpret_cast<void*>(self->m_pQuad);
    coordinates =
        ((const void*(__fastcall*)(void*, void*))
            CSF_AT(void**, quad, 0)[0x20 / 4])(quad, nullptr);
    const std::uint32_t firstX = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(CSF_AT(std::int32_t, coordinates, 0)) * scaleX));

    std::uint32_t firstPair[2] = {firstX, firstY};

    quad = reinterpret_cast<void*>(self->m_pQuad);
    coordinates =
        ((const void*(__fastcall*)(void*, void*))
            CSF_AT(void**, quad, 0)[0x28 / 4])(quad, nullptr);
    const std::uint32_t secondY = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(CSF_AT(std::int32_t, coordinates, 4)) * scaleY));

    quad = reinterpret_cast<void*>(self->m_pQuad);
    coordinates =
        ((const void*(__fastcall*)(void*, void*))
            CSF_AT(void**, quad, 0)[0x28 / 4])(quad, nullptr);
    const std::uint32_t secondX = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(CSF_AT(std::int32_t, coordinates, 0)) * scaleX));

    std::uint32_t secondPair[2] = {secondX, secondY};

    quad = reinterpret_cast<void*>(self->m_pQuad);
    ((void(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, quad, 0)[0x1C / 4])(quad, nullptr, firstPair);

    quad = reinterpret_cast<void*>(self->m_pQuad);
    return ((int(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, quad, 0)[0x24 / 4])(quad, nullptr, secondPair);
}

HOOK(0x00720EE0, CGuiStatic_SetScale, "auto");
