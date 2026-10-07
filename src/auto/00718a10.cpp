#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, GuiEffect_Create, 0x00713B20,
         (int effectType));

// Creates a QUAD_VARIABLE (type 0) effect, binds it to this control and its
// variable identifier, then registers the effect on the control (vslot +0x234).
static std::uint32_t* __fastcall CGuiControl_AddVariableQuad(
    CGuiControl* self, void* /*edx*/, int variableId)
{
    std::uint32_t* effect = GuiEffect_Create(0);
    if (!effect)
        return effect;

    // vslot +0x14: set the owning control.
    ((void(__fastcall*)(void*, void*, CGuiControl*))
        CSF_AT(void**, effect, 0)[0x14 / 4])(
            effect, nullptr, self);

    // vslot +0x6C: set the variable identifier.
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, effect, 0)[0x6C / 4])(
            effect, nullptr, variableId);

    // vslot +0x234: add the effect to the control's effect list.
    return ((std::uint32_t*(__fastcall*)(CGuiControl*, void*, std::uint32_t*))
        self->vtbl[0x234 / 4])(
            self, nullptr, effect);
}

HOOK(0x00718A10, CGuiControl_AddVariableQuad, "auto");
