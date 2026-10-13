#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor-style initializer for the auxiliary attack task object.
// Stores the class vtable, then sets the default task parameters.
// Returns the object pointer (the original returns &this->__vftable, which is the same address).
static uint32_t* __fastcall C_0083380c_InitAuxiliaryTask(C_0083380c* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0083380C);
    self->field_0004 = 0;
    self->field_000c = 1117126656;
    self->field_0010 = 1103626240;
    self->field_0014 = 10;
    self->field_0018 = 80;
    self->field_0008 = 1165623296;
    self->field_001c = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0048C810, C_0083380c_InitAuxiliaryTask, "auto");
