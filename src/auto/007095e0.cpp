#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(char, __fastcall, InitializeScreenEffect, 0x007091D0, (void* self, void* edx, int a2));
static CSF_FUNC(uint8_t*, __fastcall, ScreenEffects_FinalizeActivation, 0x00709540, (void* self, void* edx));

static char __fastcall C_00862b58_SetActivationParameter(
    C_00862b58* self, void* /*edx*/, int parameter)
{
    self->vSlot3C();
    InitializeScreenEffect(self, nullptr, parameter);
    self->vSlot2C();

    const char activated = self->vSlot40();
    if (activated)
        ScreenEffects_FinalizeActivation(self, nullptr);

    self->field_0008 = 1;
    if (!activated)
        self->vSlot3C();

    return activated;
}

HOOK(0x007095E0, C_00862b58_SetActivationParameter, "auto");
