#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, CGuiListBoxBase_SelectPreviousItem, 0x00716B80,
         (CGuiListBoxBase* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SelectNextItem, 0x00716D00,
         (CGuiListBoxBase* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiSlider_IsSelectableAndNotType10, 0x00718700,
         (void* self, void* edx));

// Capture the entire EAX forwarded by the original, not just AL.
CSF_FUNC(int, __fastcall, CGuiControl_HandleSelectionInput, 0x00718510,
         (void* self, void* edx, int action, int inputData));

static int __fastcall CGuiListBoxBase_HandleSelectionInput(
    CGuiListBoxBase* self, void* /*edx*/, int action, int inputData)
{
    int result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr);
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    result = CGuiSlider_IsSelectableAndNotType10(self, nullptr);
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    if (action == 0)
        CGuiListBoxBase_SelectPreviousItem(self, nullptr);
    else if (action == 1)
        CGuiListBoxBase_SelectNextItem(self, nullptr);

    return CGuiControl_HandleSelectionInput(self, nullptr, action, inputData);
}

HOOK(0x00716F70, CGuiListBoxBase_HandleSelectionInput, "auto");
