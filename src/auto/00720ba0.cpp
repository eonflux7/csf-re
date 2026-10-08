#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_UpdateRenderState, 0x00719A20,
         (CGuiControl* self, void* edx));
CSF_GLOBAL(float, g_Inv255, 0x00830A00);

static int __fastcall CGuiStatic_Render(CGuiStatic* self, void* /*edx*/)
{
    int result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0xAC / 4])(self, nullptr);
    if (static_cast<std::uint8_t>(result) != 0)
        return result;

    CGuiControl_UpdateRenderState(self, nullptr);

    std::uint32_t originalColor[4];
    std::uint32_t renderColor[4];
    void* object = reinterpret_cast<void*>(self->field_0138);
    if (object != nullptr)
    {
        void* color = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x6C / 4])(object, nullptr);

        originalColor[0] = CSF_AT(std::uint32_t, color, 0x00);
        originalColor[1] = CSF_AT(std::uint32_t, color, 0x04);
        originalColor[2] = CSF_AT(std::uint32_t, color, 0x08);
        originalColor[3] = CSF_AT(std::uint32_t, color, 0x0C);

        float alpha;
        std::memcpy(&alpha, &originalColor[3], sizeof(alpha));
        const double scaledAlpha =
            static_cast<double>(alpha) * self->field_0140;

        renderColor[0] = originalColor[0];
        object = reinterpret_cast<void*>(self->field_0138);
        renderColor[1] = originalColor[1];
        alpha = static_cast<float>(scaledAlpha);
        std::memcpy(&renderColor[3], &alpha, sizeof(alpha));
        renderColor[2] = originalColor[2];

        reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t*)>(
            CSF_AT(void**, object, 0)[0x68 / 4])(
                object, nullptr, renderColor);

        object = reinterpret_cast<void*>(self->field_0138);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x08 / 4])(object, nullptr);

        object = reinterpret_cast<void*>(self->field_0138);
        reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t*)>(
            CSF_AT(void**, object, 0)[0x68 / 4])(
                object, nullptr, originalColor);
    }

    object = reinterpret_cast<void*>(self->m_pQuad);
    if (object != nullptr)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, self, 0)[0x238 / 4])(self, nullptr, object);
    }

    object = reinterpret_cast<void*>(self->m_pText);
    if (object == nullptr)
        return 0;

    void* color = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x64 / 4])(object, nullptr);

    double alphaScale = static_cast<double>(self->field_0148) * g_Inv255;
    originalColor[0] = CSF_AT(std::uint32_t, color, 0x00);
    originalColor[1] = CSF_AT(std::uint32_t, color, 0x04);
    originalColor[2] = CSF_AT(std::uint32_t, color, 0x08);
    originalColor[3] = CSF_AT(std::uint32_t, color, 0x0C);
    alphaScale *= self->field_0140;

    float alpha;
    std::memcpy(&alpha, &originalColor[3], sizeof(alpha));
    const std::uint32_t textOption = self->field_0158;
    const double scaledAlpha = alphaScale * alpha;

    renderColor[0] = originalColor[0];
    object = reinterpret_cast<void*>(self->m_pText);
    renderColor[1] = originalColor[1];
    alpha = static_cast<float>(scaledAlpha);
    std::memcpy(&renderColor[3], &alpha, sizeof(alpha));
    renderColor[2] = originalColor[2];

    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
        CSF_AT(void**, object, 0)[0xBC / 4])(object, nullptr, textOption);

    object = reinterpret_cast<void*>(self->m_pText);
    const std::uint32_t secondTextOption = self->field_015c;
    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
        CSF_AT(void**, object, 0)[0xC0 / 4])(
            object, nullptr, secondTextOption);

    object = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, object, 0)[0x60 / 4])(
            object, nullptr, renderColor);

    object = reinterpret_cast<void*>(self->m_pText);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, self, 0)[0x238 / 4])(self, nullptr, object);

    object = reinterpret_cast<void*>(self->m_pText);
    return reinterpret_cast<int(__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, object, 0)[0x60 / 4])(
            object, nullptr, originalColor);
}

HOOK(0x00720BA0, CGuiStatic_Render, "auto");
