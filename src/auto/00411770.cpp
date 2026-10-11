#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Base-class reset routine (C_0082fbe8 part of the C_0082deb8 hierarchy).
CSF_FUNC(int, __fastcall, C_0082fbe8_ResetBase, 0x0040FC40, (void* self, void* edx));

// Clears the four float state values and the four byte flags of C_0082dfd0.
static int __fastcall C_0082dfd0_ClearFloatsAndFlags(C_0082dfd0* self, void* /*edx*/)
{
    C_0082fbe8_ResetBase(self, nullptr);

    self->field_00f0 = 0.0f;
    self->field_00f4 = 0.0f;
    self->field_00f8 = 0.0f;
    self->field_00fc = 0.0f;

    self->field_010c = 0;
    self->field_010d = 0;
    self->field_010e = 0;
    self->field_010f = 0;

    return 0;
}

HOOK(0x411770, C_0082dfd0_ClearFloatsAndFlags, "auto");
