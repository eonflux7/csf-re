#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static uint8_t __fastcall CGuiControl_HandleSelectionInput(
    CGuiControl* self, void* /*edx*/, int inputCode, int /*unused*/)
{
    uint8_t result = static_cast<uint8_t>(
        reinterpret_cast<int (__fastcall*)(CGuiControl*, void*)>(
            self->vtbl[0x6C / 4])(self, nullptr));
    if (!result)
        return result;

    result = static_cast<uint8_t>(
        reinterpret_cast<int (__fastcall*)(CGuiControl*, void*)>(
            self->vtbl[0xBC / 4])(self, nullptr));
    if (!result)
        return result;

    result = self->m_bSelectable;
    if (!result || inputCode != 5)
        return result;

    return static_cast<uint8_t>(
        reinterpret_cast<int (__fastcall*)(CGuiControl*, void*, int)>(
            self->vtbl[0x138 / 4])(self, nullptr, 13));
}

HOOK(0x00718510, CGuiControl_HandleSelectionInput, "auto");
