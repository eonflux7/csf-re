#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_SetParent(
    CGuiControl* self, void* /*edx*/, CGuiControl* parent)
{
    int result; // Original leaves EAX untouched when the parent is unchanged.
    auto* previousParent =
        reinterpret_cast<CGuiControl*>(self->m_pParent);

    if (parent != previousParent)
    {
        if (previousParent)
        {
            reinterpret_cast<void (__fastcall*)(
                CGuiControl*, void*, CGuiControl*)>(
                    previousParent->vtbl[0x100 / 4])(
                        previousParent, nullptr, self);
        }

        self->m_pParent = reinterpret_cast<std::uint32_t>(parent);

        if (parent)
        {
            reinterpret_cast<void (__fastcall*)(
                CGuiControl*, void*, CGuiControl*)>(
                    parent->vtbl[0xF8 / 4])(
                        parent, nullptr, self);
        }

        result = reinterpret_cast<int (__fastcall*)(
            CGuiControl*, void*, int)>(
                self->vtbl[0x138 / 4])(self, nullptr, 2);
    }

    return result;
}

HOOK(0x00718110, CGuiControl_SetParent, "auto");
