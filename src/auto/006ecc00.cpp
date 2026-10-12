#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(unsigned int, __fastcall, RefreshCollisionState, 0x006ECB70, (void* self, void* edx));

static unsigned int __fastcall C_00862028_SetCollisionState(
    C_00862028* self, void* /*edx*/, unsigned int* state)
{
    const unsigned int value = *state;
    if (self->field_0064 != value)
    {
        self->field_0064 = value;
        return RefreshCollisionState(self, nullptr);
    }
    return value;
}

HOOK(0x006ECC00, C_00862028_SetCollisionState, "auto");
