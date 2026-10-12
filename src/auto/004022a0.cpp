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

// Constructs an input binding record: stores the binding id (first stack arg)
// and clears the two flag bytes that follow it. Returns the object pointer.
static uint8_t* __fastcall InputBinding_Construct(S_004022a0* self, void* /*edx*/, int bindingId)
{
    self->field_0000 = static_cast<uint32_t>(bindingId);
    self->field_0004 = 0;
    self->field_0005 = 0;
    return reinterpret_cast<uint8_t*>(self);
}

HOOK(0x004022A0, InputBinding_Construct, "auto");
