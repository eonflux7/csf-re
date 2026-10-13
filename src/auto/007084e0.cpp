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

// Base-class constructor C_00860178 (0x6B2820), __fastcall-style: ecx = this, edx unused.
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));

// Constructs a C_00863430 in place (base ctor first, then zeroed fields and default
// values), and returns the object's address as a pointer to its vtable slot.
static std::uint32_t* __fastcall C_00863430_Construct(C_00863430* self, void* /*edx*/)
{
    C_00860178_Construct(self, nullptr);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0018 = 0;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00863430);
    self->field_0014 = 6;
    self->field_0010 = 1.0f;
    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x007084E0, C_00863430_Construct, "auto");
