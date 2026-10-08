#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __fastcall, ProcessAnimatedQuad, 0x00731C70,
         (void* self, void* edx, void* quad));
CSF_FUNC(int, __fastcall, UpdateButtonQuadState, 0x00725B40,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiButton_ProcessQuadWithScaledAlpha2, 0x00725010,
         (void* self, void* edx, void* quad, float alpha));

CSF_GLOBAL(float, g_fInv255, 0x00830A00);

static void __fastcall CGuiButton_UpdateQuad(void* self, void* /*edx*/)
{
    void* quad = CSF_AT(void*, self, 0x134);
    if (!quad)
        return;

    if (ProcessAnimatedQuad(CSF_AT(void*, self, 0x1D4), nullptr, quad))
        return;

    // The position getter uses the last two words of the rectangle scratch area.
    std::uint32_t stateRectangle[4];
    void** quadVtable = CSF_AT(void**, CSF_AT(void*, self, 0x134), 0);
    void* position =
        reinterpret_cast<void* (__fastcall*)(void*, void*, std::uint32_t*)>(
            CSF_AT(void**, self, 0)[0x160 / 4])(
                self, nullptr, &stateRectangle[2]);

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        quadVtable[0x50 / 4])(
            CSF_AT(void*, self, 0x134), nullptr, position);

    quadVtable = CSF_AT(void**, CSF_AT(void*, self, 0x134), 0);
    void* size =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        quadVtable[0x58 / 4])(
            CSF_AT(void*, self, 0x134), nullptr, size);

    const char specialState =
        reinterpret_cast<char (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr);

    if (!specialState)
    {
        const std::uint32_t stateOffset =
            CSF_AT(std::uint32_t, self, 0x148) * 0x1Cu;
        auto* rectangle = reinterpret_cast<std::uint8_t*>(self)
                        + (stateOffset + 0x158u);

        stateRectangle[0] = CSF_AT(std::uint32_t, rectangle, 0);
        stateRectangle[1] = CSF_AT(std::uint32_t, rectangle, 4);
        stateRectangle[2] = CSF_AT(std::uint32_t, rectangle, 8);
        stateRectangle[3] = CSF_AT(std::uint32_t, rectangle, 0xC);

        quad = CSF_AT(void*, self, 0x134);
        reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
            CSF_AT(void**, quad, 0)[0x60 / 4])(
                quad, nullptr, stateRectangle);

        const float alpha = CSF_AT(float, self, 0x1D8);
        quad = CSF_AT(void*, self, 0x134);
        const float scaledAlpha = static_cast<float>(
            static_cast<double>(alpha) * static_cast<double>(g_fInv255));
        CGuiButton_ProcessQuadWithScaledAlpha2(
            self, nullptr, quad, scaledAlpha);
        return;
    }

    UpdateButtonQuadState(self, nullptr);
    CGuiButton_ProcessQuadWithScaledAlpha2(
        self, nullptr, CSF_AT(void*, self, 0x134), 1.0f);

    const std::uint32_t stateOffset =
        CSF_AT(std::uint32_t, self, 0x148) * 0x1Cu;
    auto* rectangle = reinterpret_cast<std::uint8_t*>(self)
                    + (stateOffset + 0x158u);

    stateRectangle[0] = CSF_AT(std::uint32_t, rectangle, 0);
    stateRectangle[1] = CSF_AT(std::uint32_t, rectangle, 4);
    stateRectangle[2] = CSF_AT(std::uint32_t, rectangle, 8);
    stateRectangle[3] = CSF_AT(std::uint32_t, rectangle, 0xC);

    void* selectedRectangle =
        reinterpret_cast<std::uint8_t*>(self) + 0x1C0;
    if (CSF_AT(std::uint32_t, self, 0x1C0) == 0xBF800000u)
        selectedRectangle = stateRectangle;

    std::uint32_t finalRectangle[4];
    finalRectangle[0] = CSF_AT(std::uint32_t, selectedRectangle, 0);
    finalRectangle[1] = CSF_AT(std::uint32_t, selectedRectangle, 4);
    finalRectangle[2] = CSF_AT(std::uint32_t, selectedRectangle, 8);
    finalRectangle[3] = CSF_AT(std::uint32_t, selectedRectangle, 0xC);

    quad = CSF_AT(void*, self, 0x134);
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, quad, 0)[0x60 / 4])(
            quad, nullptr, finalRectangle);
}

HOOK(0x00726210, CGuiButton_UpdateQuad, "auto");
