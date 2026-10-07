#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static std::uint8_t __fastcall CGuiControl_HandleMouseMovement(
    CGuiControl* self, void* /*edx*/, void* mouseEvent)
{
    const std::uint32_t mouseX = CSF_AT(std::uint32_t, mouseEvent, 0x14);
    const std::uint32_t mouseY = CSF_AT(std::uint32_t, mouseEvent, 0x18);

    std::uint32_t coordinateBuffer[3];
    auto* position =
        ((std::uint32_t*(__fastcall*)(void*, void*, void*))
             self->vtbl[0x160 / 4])(self, nullptr, coordinateBuffer);

    const std::uint32_t relativeY = mouseY - position[1];
    const std::uint32_t relativeX = mouseX - position[0];
    std::uint32_t relativePosition[2] = {relativeX, relativeY};

    ((void(__fastcall*)(void*, void*, std::uint32_t*, int))
         self->vtbl[0x14c / 4])(self, nullptr, relativePosition, 1);

    const std::uint8_t active =
        ((std::uint8_t(__fastcall*)(void*, void*))
             self->vtbl[0x6c / 4])(self, nullptr);
    if (!active)
        return active;
    if (!self->m_bSelectable)
        return 0;
    if (!self->m_bMouseEnabled)
        return 0;

    const std::uint32_t inputState = self->m_inputState;
    if (inputState & 0x10)
    {
        const std::uint32_t targetX = self->field_00cc + mouseX;
        const std::uint32_t targetY = self->field_00d0 + mouseY;

        auto* origin =
            ((std::uint32_t*(__fastcall*)(void*, void*, void*))
                 self->vtbl[0x220 / 4])(self, nullptr, coordinateBuffer);

        const std::uint32_t originY = origin[1];
        const std::uint32_t originX = origin[0];
        void** vtable = self->vtbl;
        relativePosition[0] = targetX - originX;
        relativePosition[1] = targetY - originY;

        return ((std::uint8_t(__fastcall*)(void*, void*, std::uint32_t*))
                    vtable[0x98 / 4])(self, nullptr, relativePosition);
    }

    if (!(inputState & 1))
        return static_cast<std::uint8_t>(inputState);

    if (!(inputState & 4))
    {
        return ((std::uint8_t(__fastcall*)(void*, void*, int))
                    self->vtbl[0x138 / 4])(self, nullptr, 17);
    }

    if (!self->m_bDraggable)
        return static_cast<std::uint8_t>(inputState);

    void** vtable = self->vtbl;
    self->m_inputState = inputState | 0x10;
    position =
        ((std::uint32_t*(__fastcall*)(void*, void*, void*))
             vtable[0x160 / 4])(self, nullptr, coordinateBuffer);

    const std::uint32_t positionX = position[0];
    const std::uint32_t positionY = position[1];
    vtable = self->vtbl;
    self->field_00cc = positionX - mouseX;
    self->field_00d0 = positionY - mouseY;

    return ((std::uint8_t(__fastcall*)(void*, void*, int))
                vtable[0x138 / 4])(self, nullptr, 4);
}

HOOK(0x00719400, CGuiControl_HandleMouseMovement, "auto");
