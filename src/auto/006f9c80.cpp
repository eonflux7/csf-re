#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint8_t*, __fastcall, ScreenEffects_FinalizeActivation, 0x00709540, (void* self, void* edx));
static CSF_FUNC(char, __fastcall, ScreenEffects_SetActivationParameter, 0x007095E0, (void* self, void* edx, int a2));

static char __fastcall C_00862b58_Activate(
    C_00862b58* self, void* /*edx*/, int parameter)
{
    self->vSlot3C();
    ScreenEffects_SetActivationParameter(self, nullptr, parameter);
    self->vSlot2C();

    const char activated = self->vSlot40();
    if (activated)
        ScreenEffects_FinalizeActivation(self, nullptr);

    self->field_0008 = 1;
    if (!activated)
        self->vSlot3C();

    return activated;
}

HOOK(0x006F9C80, C_00862b58_Activate, "auto");
