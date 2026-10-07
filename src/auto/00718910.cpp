#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, GuiEffect_Create, 0x00713B20,
         (int effectType));

static std::uint32_t* __fastcall CGuiControl_AddFocusQuad(
    CGuiControl* self, void* /*edx*/, int focusParameter)
{
    std::uint32_t* effect = GuiEffect_Create(3);
    if (!effect)
        return effect;

    ((void(__fastcall*)(void*, void*, CGuiControl*))
        CSF_AT(void**, effect, 0)[0x14 / 4])(
            effect, nullptr, self);

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, effect, 0)[0x5C / 4])(
            effect, nullptr, focusParameter);

    return ((std::uint32_t*(__fastcall*)(CGuiControl*, void*, std::uint32_t*))
        self->vtbl[0x234 / 4])(
            self, nullptr, effect);
}

HOOK(0x00718910, CGuiControl_AddFocusQuad, "auto");
