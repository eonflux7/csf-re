#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall CGuiControl_OnKeyDown(
    CGuiControl* self, void* /*edx*/, int keyCode, int /*unused*/)
{
    char result = static_cast<char>(
        ((int(__fastcall*)(CGuiControl*, void*))self->vtbl[0x6C / 4])(
            self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(
        ((int(__fastcall*)(CGuiControl*, void*))self->vtbl[0xBC / 4])(
            self, nullptr));
    if (!result)
        return result;

    result = static_cast<char>(self->m_bSelectable);
    if (!result)
        return result;

    result = static_cast<char>(keyCode);
    if (keyCode == 0x1C || keyCode == 0x9C)
    {
        result = static_cast<char>(
            ((int(__fastcall*)(CGuiControl*, void*, int))self->vtbl[0x138 / 4])(
                self, nullptr, 13));
    }
    return result;
}

HOOK(0x007184C0, CGuiControl_OnKeyDown, "auto");
