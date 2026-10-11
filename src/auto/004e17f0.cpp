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

// Base-class initializer at 0x4E1200 (__thiscall, 2 stack args: a2, a3).
CSF_FUNC(uint32_t*, __fastcall, sub_4E1200, 0x004E1200, (void* self, void* edx, int a2, int a3));

// Constructs C_0083e418: runs the base initializer with flag 1, installs the
// C_0083e418 vtable and the secondary vtable slot, stores the constructor
// argument, and returns the object pointer.
static uint32_t* __fastcall C_0083e418_Construct(C_0083e418* self, void* /*edx*/, int a2, int a3)
{
    sub_4E1200(self, nullptr, a2, 1);
    self->vtbl = CSF_VTBL(0x0083E418);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0083E3FC));
    self->field_0018 = static_cast<uint32_t>(a3);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004E17F0, C_0083e418_Construct, "auto");
