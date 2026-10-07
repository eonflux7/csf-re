#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static bool __fastcall CGuiControl_ContainsPoint(
    CGuiControl* self, void* /*edx*/, const std::int32_t* point)
{
    std::int32_t origin[2];
    ((void(__fastcall*)(CGuiControl*, void*, std::int32_t*))
        self->vtbl[0x160 / 4])(self, nullptr, origin);

    const std::int32_t x = point[0];
    if (x < origin[0])
        return false;

    const std::int32_t right = static_cast<std::int32_t>(
        static_cast<std::uint32_t>(origin[0]) + self->m_width);
    if (x >= right)
        return false;

    const std::int32_t y = point[1];
    if (y < origin[1])
        return false;

    const std::int32_t bottom = static_cast<std::int32_t>(
        static_cast<std::uint32_t>(origin[1]) + self->m_height);
    return y < bottom;
}

HOOK(0x007185A0, CGuiControl_ContainsPoint, "auto");
