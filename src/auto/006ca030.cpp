#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies this object's three child-slot fields (+0x10, +0x14, +0x18) into the
// destination block at a2[4], a2[5], a2[6] and returns the destination.
static uint32_t* __fastcall C_00862598_SetChildObject(C_00862598* self, void* /*edx*/, uint32_t* a2)
{
    a2[5] = self->field_0014;
    a2[4] = self->field_0010;
    a2[6] = self->field_0018;
    return a2;
}

HOOK(0x6CA030, C_00862598_SetChildObject, "auto");
