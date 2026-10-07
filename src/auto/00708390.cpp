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

// Returns the dword at offset 4 of the object referenced by this->field_0004.
static int __fastcall C_00863430_GetLinkedValue(C_00863430* self, void* /*edx*/)
{
    return static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x4));
}

HOOK(0x00708390, C_00863430_GetLinkedValue, "auto");
