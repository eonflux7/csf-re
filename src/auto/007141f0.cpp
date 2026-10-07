#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_IsInputAllowed, 0x00718700,
         (CGuiControl* self, void* edx));

static int __fastcall CGuiListBoxBase_ClearControlKeyState(
    CGuiListBoxBase* self, void* /*edx*/, int keyCode, int /*unused*/)
{
    int result =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);
    if (static_cast<uint8_t>(result) == 0)
        return result;

    result =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr);
    if (static_cast<uint8_t>(result) == 0)
        return result;

    result = CGuiControl_IsInputAllowed(self, nullptr);
    if (static_cast<uint8_t>(result) == 0)
        return result;

    if (keyCode == 29 || keyCode == 157)
        self->field_0164 = 0;

    return keyCode;
}

HOOK(0x007141F0, CGuiListBoxBase_ClearControlKeyState, "auto");
