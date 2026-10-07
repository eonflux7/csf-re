#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, GuiEffect_Create, 0x00713B20,
         (int effectType));

// Creates a QUAD_BLINKING_FOCUS effect (type 4), assigns the owning control,
// stores the blink parameter, and registers the effect on the control.
static std::uint32_t* __fastcall CGuiControl_AddBlinkingFocusQuad(
    CGuiControl* self, void* /*edx*/, int blinkParameter)
{
    std::uint32_t* effect = GuiEffect_Create(4);
    if (!effect)
        return effect;

    // effect->vslot[0x14/4](self): set the owner control
    ((void(__fastcall*)(void*, void*, CGuiControl*))
        CSF_AT(void**, effect, 0)[0x14 / 4])(
            effect, nullptr, self);

    // effect->vslot[0x68/4](blinkParameter)
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, effect, 0)[0x68 / 4])(
            effect, nullptr, blinkParameter);

    // self->vslot[0x234/4](effect): attach the effect, result returned to caller
    return ((std::uint32_t*(__fastcall*)(CGuiControl*, void*, std::uint32_t*))
        self->vtbl[0x234 / 4])(
            self, nullptr, effect);
}

HOOK(0x00718950, CGuiControl_AddBlinkingFocusQuad, "auto");
