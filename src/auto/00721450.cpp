#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CGuiControl_ResetInput, 0x00717F70,
         (CGuiControl* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_ForwardEventToFirstEffect, 0x00718C00,
         (CGuiControl* self, void* edx, int event));
CSF_FUNC(int*, __fastcall, EraseWideStringRange, 0x004F21D0,
         (legacy::wstring* self, void* edx, int* result,
          int first, unsigned int last));
CSF_FUNC(int, __fastcall, CGuiEdit_ClearTextAndPositions, 0x007213C0,
         (CGuiEdit* self, void* edx));

static int __fastcall CGuiEdit_Reset(CGuiEdit* self, void* /*edx*/)
{
    CGuiControl_ResetInput(static_cast<CGuiControl*>(self), nullptr);

    self->field_014c = 1.0f;
    CGuiControl_ForwardEventToFirstEffect(static_cast<CGuiControl*>(self),
                             nullptr, 0x3F800000);

    const std::uint32_t capacity =
        CSF_AT(std::uint32_t, self, 0x1A0);
    const std::uintptr_t inlineBuffer =
        reinterpret_cast<std::uintptr_t>(
            &CSF_AT(std::uint16_t, self, 0x18C));

    std::uint32_t last =
        capacity < 8u
            ? static_cast<std::uint32_t>(inlineBuffer)
            : CSF_AT(std::uint32_t, self, 0x18C);
    last += 2u * CSF_AT(std::uint32_t, self, 0x19C);

    const std::uint32_t first =
        capacity < 8u
            ? static_cast<std::uint32_t>(inlineBuffer)
            : CSF_AT(std::uint32_t, self, 0x18C);

    int eraseResult;
    EraseWideStringRange(&self->m_wtext2, nullptr, &eraseResult,
                        static_cast<int>(first), last);

    CGuiEdit_ClearTextAndPositions(self, nullptr);

    void* text = reinterpret_cast<void*>(self->m_pText);
    if (text)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, text, 0)[0x3C / 4])(
                text, nullptr, CSF_STR(0x0082E65C, ""));
    }

    self->field_01b4 = 0xFFFFFFFFu;
    self->field_01b8 = 0.0f;
    self->field_01bc = 0xFFFFFFFFu;
    self->field_01c0 = 16;
    return -1;
}

HOOK(0x00721450, CGuiEdit_Reset, "auto");
