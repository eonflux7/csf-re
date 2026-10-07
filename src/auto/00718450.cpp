#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static uint8_t __fastcall CGuiControl_ProcessSelectionInput(
    CGuiControl* self, void* /*edx*/, int /*unused*/)
{
    uint8_t result = static_cast<uint8_t>(
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            self->vtbl[0x6C / 4])(self, nullptr));
    if (result == 0)
        return result;

    result = self->m_bMouseEnabled;
    if (result == 0)
        return result;

    result = self->m_bSelectable;
    if (result == 0)
        return result;

    const uint32_t inputState = self->m_inputState;
    const uint32_t pendingSelection = inputState & 8u;

    if (pendingSelection != 0 && (inputState & 1u) != 0)
    {
        void** eventVtable = self->vtbl;
        self->m_inputState = inputState & 0xFFFFFFF7u;
        return static_cast<uint8_t>(
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                eventVtable[0x138 / 4])(self, nullptr, 22));
    }

    if (pendingSelection != 0 && (inputState & 2u) != 0)
    {
        self->m_inputState = inputState & 0xFFFFFFF7u;
        return static_cast<uint8_t>(
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                self->vtbl[0x138 / 4])(self, nullptr, 22));
    }

    return static_cast<uint8_t>(inputState);
}

HOOK(0x00718450, CGuiControl_ProcessSelectionInput, "auto");
