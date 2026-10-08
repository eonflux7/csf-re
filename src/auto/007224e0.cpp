#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AdjustTextRangeLeft, 0x007215F0,
         (CGuiEdit* self, void* edx, char argument));

// Keep register allocation minimal: the original helper implicitly receives EBX.
static __attribute__((optnone)) int __fastcall
CGuiEdit_MoveIndicesToTextEnd(CGuiEdit* self, void* /*edx*/)
{
    void* text = reinterpret_cast<void*>(self->m_pText);
    int result =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, text, 0)[0x88 / 4])(text, nullptr);

    if (self->field_01ac != 0)
        return result;

    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    const std::uint32_t length = CSF_AT(std::uint32_t, self, 0x164);
    if (length == 0)
        return 0;

    const std::uint32_t lastIndex = length - 1;
    self->m_caretIndex = lastIndex;
    self->field_01b0 = lastIndex;

    return AdjustTextRangeLeft(self, nullptr, 0);
}

HOOK(0x007224E0, CGuiEdit_MoveIndicesToTextEnd, "auto");
