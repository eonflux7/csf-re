#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiListBoxBase_SelectPreviousItem, 0x00716B80,
         (CGuiListBoxBase* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SelectNextItem, 0x00716D00,
         (CGuiListBoxBase* self, void* edx));

static int __fastcall CGuiListBoxBase_HandleWheelInput(
    CGuiListBoxBase* self, void* /*edx*/, void* input)
{
    int result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr);
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    result = CSF_AT(int, input, 0x08);
    if (result > 0)
        return CGuiListBoxBase_SelectPreviousItem(self, nullptr);
    if (result < 0)
        return CGuiListBoxBase_SelectNextItem(self, nullptr);
    return result;
}

HOOK(0x00716E80, CGuiListBoxBase_HandleWheelInput, "auto");
