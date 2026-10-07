#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_CenterInParent(
    CGuiControl* self, void* /*edx*/)
{
    auto* parent = reinterpret_cast<CGuiControl*>(self->m_pParent);
    if (!parent)
        return 0;

    const auto* parentSize =
        reinterpret_cast<const std::int32_t* (__fastcall*)(CGuiControl*, void*)>(
            parent->vtbl[0xA4 / 4])(parent, nullptr);

    std::int32_t position[2];

    const auto width = static_cast<std::int32_t>(self->m_width);
    const auto parentWidth = parentSize[0];
    position[0] = parentWidth / 2 - width / 2;

    const auto height = static_cast<std::int32_t>(self->m_height);
    const auto parentHeight = parentSize[1];
    position[1] = parentHeight / 2 - height / 2;

    return reinterpret_cast<int (__fastcall*)(
        CGuiControl*, void*, std::int32_t*)>(
            self->vtbl[0x98 / 4])(self, nullptr, position);
}

HOOK(0x00718FE0, CGuiControl_CenterInParent, "auto");
