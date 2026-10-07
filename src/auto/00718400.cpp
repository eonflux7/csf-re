#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint8_t __fastcall CGuiControl_HandleSelectionInput(
    CGuiControl* self, void* /*edx*/, int /*unused*/)
{
    uint8_t result = static_cast<uint8_t>(
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
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
    if ((inputState & 1u) == 0)
        return static_cast<uint8_t>(inputState);

    void** eventVtable = self->vtbl;
    self->m_inputState = inputState | 8u;

    return static_cast<uint8_t>(
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            eventVtable[0x138 / 4])(self, nullptr, 21));
}

HOOK(0x00718400, CGuiControl_HandleSelectionInput, "auto");
