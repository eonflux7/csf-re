#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint8_t __fastcall CGuiControl_HandleSelectionInput(
    CGuiControl* self, void* /*edx*/, void* inputEvent)
{
    uint8_t result = static_cast<uint8_t>(
        reinterpret_cast<int (__fastcall*)(CGuiControl*, void*)>(
            self->vtbl[0x6C / 4])(self, nullptr));
    if (!result)
        return result;

    result = self->m_bMouseEnabled;
    if (!result)
        return result;

    result = self->m_bSelectable;
    if (!result)
        return result;

    const uint32_t inputState = self->m_inputState;
    if (!(inputState & 1))
        return static_cast<uint8_t>(inputState);

    self->m_inputState = inputState | 4;

    const int eventId = CSF_AT(uint8_t, inputEvent, 0x1F) ? 20 : 18;
    return static_cast<uint8_t>(
        reinterpret_cast<int (__fastcall*)(CGuiControl*, void*, int)>(
            self->vtbl[0x138 / 4])(self, nullptr, eventId));
}

HOOK(0x00718300, CGuiControl_HandleSelectionInput, "auto");
