#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_UpdateMouseHover(
    CGuiControl* self, void* /*edx*/, const std::uint32_t* pointerPosition,
    char mouseActive)
{
    self->field_00d8 = pointerPosition[0];
    self->field_00dc = pointerPosition[1];

    if (!mouseActive ||
        !reinterpret_cast<std::uint8_t(__fastcall*)(void*, void*)>(
            self->vtbl[0x6C / 4])(self, nullptr) ||
        !self->m_bMouseEnabled ||
        !self->m_bSelectable)
    {
        const std::uint32_t state = self->m_inputState;
        if (!(state & 1u))
            return static_cast<int>(state);

        void** vtable = self->vtbl;
        self->m_inputState = (state & ~1u) | 2u;
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            vtable[0x138 / 4])(self, nullptr, 16);
    }

    std::uint32_t* position =
        reinterpret_cast<std::uint32_t*(__fastcall*)(void*, void*)>(
            self->vtbl[0x148 / 4])(self, nullptr);

    std::uint32_t offsetBuffer[2];
    std::uint32_t* offset =
        reinterpret_cast<std::uint32_t*(__fastcall*)(
            void*, void*, std::uint32_t*)>(
                self->vtbl[0x160 / 4])(self, nullptr, offsetBuffer);

    const std::uint32_t offsetY = offset[1];
    const std::uint32_t positionY = position[1];
    const std::uint32_t combinedY = positionY + offsetY;
    const std::uint32_t positionX = position[0];
    const std::uint32_t offsetX = offset[0];

    std::uint32_t hitPosition[2];
    hitPosition[1] = combinedY;
    hitPosition[0] = positionX + offsetX;

    const std::uint8_t hit =
        reinterpret_cast<std::uint8_t(__fastcall*)(
            void*, void*, std::uint32_t*)>(
                self->vtbl[0x154 / 4])(self, nullptr, hitPosition);

    const std::uint32_t state = self->m_inputState;
    if (hit)
    {
        if (!(state & 2u))
            return static_cast<int>(state);

        void** vtable = self->vtbl;
        self->m_inputState = (state & ~2u) | 1u;
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            vtable[0x138 / 4])(self, nullptr, 15);
    }

    if (!(state & 1u))
        return static_cast<int>(state);

    self->m_inputState = (state & ~1u) | 2u;
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        self->vtbl[0x138 / 4])(self, nullptr, 16);
}

HOOK(0x00718DE0, CGuiControl_UpdateMouseHover, "auto");
