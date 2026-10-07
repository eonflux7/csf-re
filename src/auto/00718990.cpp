#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Creates a QUAD_BACKGROUND effect (0x713B20), binds it to this control and to
// the given owner/renderer argument, then registers it on the control through
// vtable slot +0x234.
CSF_FUNC(std::uint32_t*, __cdecl, GuiEffect_Create, 0x00713B20,
         (int effectType));

static std::uint32_t* __fastcall CGuiControl_AddBackgroundQuad(
    CGuiControl* self, void* /*edx*/, int backgroundParameter)
{
    std::uint32_t* effect = GuiEffect_Create(1);
    if (!effect)
        return effect;

    // effect->vtable[0x14/4](self)  -- attach the owning control
    ((void(__fastcall*)(void*, void*, CGuiControl*))
        CSF_AT(void**, effect, 0)[0x14 / 4])(
            effect, nullptr, self);

    // effect->vtable[0x34/4](backgroundParameter)
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, effect, 0)[0x34 / 4])(
            effect, nullptr, backgroundParameter);

    // self->vtable[0x234/4](effect)  -- add the effect to the control
    return ((std::uint32_t*(__fastcall*)(CGuiControl*, void*, std::uint32_t*))
        self->vtbl[0x234 / 4])(
            self, nullptr, effect);
}

HOOK(0x00718990, CGuiControl_AddBackgroundQuad, "auto");
