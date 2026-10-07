#include <cstdint>
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_SetColor(
    CGuiControl* self, void* /*edx*/, const void* color)
{
    const auto* components = static_cast<const std::uint8_t*>(color);
    std::uint32_t bits;

    // Copy each component sequentially, preserving its exact bit pattern.
    std::memcpy(&bits, components, 4);
    std::memcpy(&self->m_colorR, &bits, 4);

    std::memcpy(&bits, components + 4, 4);
    std::memcpy(&self->m_colorG, &bits, 4);

    std::memcpy(&bits, components + 8, 4);
    std::memcpy(&self->m_colorB, &bits, 4);

    std::memcpy(&bits, components + 12, 4);
    std::memcpy(&self->m_colorA, &bits, 4);

    return reinterpret_cast<int (__fastcall*)(CGuiControl*, void*, int)>(
        self->vtbl[0x138 / 4])(self, nullptr, 6);
}

HOOK(0x00718170, CGuiControl_SetColor, "auto");
