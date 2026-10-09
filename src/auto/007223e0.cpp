#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t*, __fastcall, MenuEula_ComputeWriteCursorForEndPointer, 0x00532CB0, (void* thisPtr, void* , uint32_t* out, uint32_t endAddr));
CSF_FUNC(int, __fastcall, AdjustTextRangeRight, 0x00721900, (CGuiEdit* self, void* edx, char mode));

static int __fastcall CGuiEdit_DeleteAtTextPosition(CGuiEdit* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x164) == 0)
        return 0;

    auto* textControl = reinterpret_cast<CGuiControl*>(self->m_pText);
    const uint8_t textFlag =
        static_cast<uint8_t>(reinterpret_cast<uintptr_t>(textControl->vGetIdLeft()));

    const char mode = (self->field_01ac || !textFlag) ? 0 : 1;

    uint32_t endAddress;
    if (CSF_AT(uint32_t, self, 0x168) < 8u)
        endAddress = reinterpret_cast<uint32_t>(&CSF_AT(uint16_t, self, 0x154));
    else
        endAddress = CSF_AT(uint32_t, self, 0x154);

    if (mode == 0)
        endAddress += 2 * self->m_caretIndex + 2;

    uint32_t writeCursor;
    MenuEula_ComputeWriteCursorForEndPointer(
        &self->m_wtext0, nullptr, &writeCursor, endAddress);
    return AdjustTextRangeRight(self, nullptr, mode);
}

HOOK(0x007223E0, CGuiEdit_DeleteAtTextPosition, "auto");
