#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(char, __fastcall, ScreenEffects_UpdateBase, 0x007090F0, (void* self, void* edx));

static char __fastcall C_00862b04_Update(C_00862b04* self, void* /*edx*/)
{
    if (self->field_0008 && self->field_0060)
        self->field_0060 = 0;

    return ScreenEffects_UpdateBase(self, nullptr);
}

HOOK(0x006F9820, C_00862b04_Update, "auto");
