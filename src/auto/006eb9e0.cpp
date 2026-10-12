#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall C_00862028_SetStateByte(
    C_00862028* self, void* /*edx*/, char state)
{
    if (self->field_0004 != static_cast<uint8_t>(state))
        self->field_0004 = static_cast<uint8_t>(state);
    return state;
}

HOOK(0x006EB9E0, C_00862028_SetStateByte, "auto");
