#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static C_00862028* __fastcall C_00862028_Initialize(C_00862028* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00862028);
    CSF_AT(void*, self, 0x8) = CSF_VTBL(0x0086200C);
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;
    self->field_0020 = 1;
    self->field_0021 = 0;
    self->field_0040 = 1;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 10;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 10;
    self->field_0064 = 0;
    self->field_0004 = 0;
    self->field_0064 = 0;
    return self;
}

HOOK(0x006ED0C0, C_00862028_Initialize, "auto");
