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
CSF_FUNC(int, __fastcall, CGuiControl_OnKeyDown, 0x007184C0,
         (void* self, void* edx, int key, int inputData));

static int __fastcall CGuiListBoxBase_OnKeyDown(
    CGuiListBoxBase* self, void* /*edx*/, int key, int inputData)
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

    switch (key)
    {
    case 29:
    case 157:
        self->field_0164 = 1;
        break;
    case 200:
        CGuiListBoxBase_SelectPreviousItem(self, nullptr);
        break;
    case 208:
        CGuiListBoxBase_SelectNextItem(self, nullptr);
        break;
    }

    return CGuiControl_OnKeyDown(self, nullptr, key, inputData);
}

HOOK(0x00716ED0, CGuiListBoxBase_OnKeyDown, "auto");
