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

// Constructor body of C_0082e520 (wheel/vehicle helper): installs the vtable,
// zeroes the two pointer-sized fields, and sets the byte flag at +0x0C to 1.
static uint32_t* __fastcall C_0082e520_Initialize(C_0082e520* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0082E520);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 1;
    return reinterpret_cast<uint32_t*>(&self->vtbl);
}

HOOK(0x0041E0F0, C_0082e520_Initialize, "auto");
