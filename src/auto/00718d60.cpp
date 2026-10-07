#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_SetPosition(
    CGuiControl* self, void* /*edx*/, const std::int32_t* position)
{
    const std::uint32_t requestedX = static_cast<std::uint32_t>(position[0]);
    const std::uint32_t requestedY = static_cast<std::uint32_t>(position[1]);
    const std::uint32_t oldY = self->m_y;
    const std::uint32_t oldX = self->m_x;
    const std::uint32_t deltaY = oldY - requestedY;

    self->m_x = requestedX;
    const std::uint32_t deltaX = oldX - requestedX;
    self->m_y = static_cast<std::uint32_t>(position[1]);

    const auto coordinates =
        reinterpret_cast<const std::uint32_t* (__fastcall*)(void*, void*)>(
            self->vtbl[0x148 / 4])(self, nullptr);

    const std::uint32_t coordinateY = coordinates[1];
    const std::uint32_t coordinateX = coordinates[0];
    const auto setCoordinates =
        reinterpret_cast<void (__fastcall*)(
            void*, void*, std::uint32_t*, int)>(
                self->vtbl[0x14C / 4]);

    std::uint32_t adjustedCoordinates[2];
    adjustedCoordinates[1] = coordinateY + deltaY;
    adjustedCoordinates[0] = coordinateX + deltaX;
    setCoordinates(self, nullptr, adjustedCoordinates, 1);

    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        self->vtbl[0x138 / 4])(self, nullptr, 2);
}

HOOK(0x00718D60, CGuiControl_SetPosition, "auto");
