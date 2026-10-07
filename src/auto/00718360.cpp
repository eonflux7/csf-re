#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall CGuiControl_ProcessSelectionInput(
    CGuiControl* self, void* /*edx*/, int /*unused*/)
{
    using QueryMethod = int (__fastcall*)(void*, void*);
    using SendEventMethod = int (__fastcall*)(void*, void*, int);

    const auto queryResult = static_cast<uint8_t>(
        reinterpret_cast<QueryMethod>(self->vtbl[0x6C / 4])(self, nullptr));

    if (queryResult == 0)
        return 0;
    if (self->m_bMouseEnabled == 0)
        return 0;
    if (self->m_bSelectable == 0)
        return 0;

    const uint32_t inputState = self->m_inputState;

    if ((inputState & 0x10u) != 0)
    {
        void** vtable = self->vtbl;
        self->m_inputState = inputState & 0xFFFFFFEBu;
        reinterpret_cast<SendEventMethod>(vtable[0x138 / 4])(
            self, nullptr, 19);

        return static_cast<char>(
            reinterpret_cast<SendEventMethod>(self->vtbl[0x138 / 4])(
                self, nullptr, 5));
    }

    if ((inputState & 4u) != 0 && (inputState & 1u) != 0)
    {
        void** vtable = self->vtbl;
        self->m_inputState = inputState & 0xFFFFFFFBu;
        return static_cast<char>(
            reinterpret_cast<SendEventMethod>(vtable[0x138 / 4])(
                self, nullptr, 19));
    }

    if ((inputState & 4u) != 0 && (inputState & 2u) != 0)
    {
        self->m_inputState = inputState & 0xFFFFFFFBu;
        return static_cast<char>(
            reinterpret_cast<SendEventMethod>(self->vtbl[0x138 / 4])(
                self, nullptr, 19));
    }

    return static_cast<char>(inputState);
}

HOOK(0x00718360, CGuiControl_ProcessSelectionInput, "auto");
