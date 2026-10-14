#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// The original returns the whole eax on every path: the value of vIsShown
// (only its low byte is tested), a `mov al, byte ptr [...]` keeps the upper
// 24 bits of that value, m_inputState is loaded as a dword, and the virtual
// callees' results are passed through untouched.
static std::uint32_t __fastcall CGuiControl_HandleMouseMovement(
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

    // eax: the full return value of vIsShown.
    std::uint32_t eax =
        ((std::uint32_t(__fastcall*)(void*, void*))
             self->vtbl[0x6c / 4])(self, nullptr);
    if (static_cast<std::uint8_t>(eax) == 0)
        return eax;

    // mov al, byte ptr [esi + 0x115]
    eax = (eax & 0xFFFFFF00u) | self->m_bSelectable;
    if (static_cast<std::uint8_t>(eax) == 0)
        return eax;

    // mov al, byte ptr [esi + 0xca]
    eax = (eax & 0xFFFFFF00u) | self->m_bMouseEnabled;
    if (static_cast<std::uint8_t>(eax) == 0)
        return eax;

    // mov eax, dword ptr [esi + 0xe0]
    eax = self->m_inputState;
    if ((eax & 0x10) != 0)
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

        return ((std::uint32_t(__fastcall*)(void*, void*, std::uint32_t*))
                    vtable[0x98 / 4])(self, nullptr, relativePosition);
    }

    if ((eax & 1) == 0)
        return eax;

    if ((eax & 4) == 0)
    {
        return ((std::uint32_t(__fastcall*)(void*, void*, int))
                    self->vtbl[0x138 / 4])(self, nullptr, 17);
    }

    if (self->m_bDraggable == 0)
        return eax;

    void** vtable = self->vtbl;
    self->m_inputState = eax | 0x10;
    position =
        ((std::uint32_t*(__fastcall*)(void*, void*, void*))
             vtable[0x160 / 4])(self, nullptr, coordinateBuffer);

    const std::uint32_t positionX = position[0];
    const std::uint32_t positionY = position[1];
    vtable = self->vtbl;
    self->field_00cc = positionX - mouseX;
    self->field_00d0 = positionY - mouseY;

    return ((std::uint32_t(__fastcall*)(void*, void*, int))
                vtable[0x138 / 4])(self, nullptr, 4);
}

HOOK(0x00719400, CGuiControl_HandleMouseMovement, "auto");
