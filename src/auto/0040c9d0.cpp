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

// Concurrency::details::VirtualProcessor::GetExecutionResourceId
// __thiscall, no stack arguments, returns the execution resource id stored at +0x8C.
static unsigned int __fastcall VirtualProcessor_GetExecutionResourceId(C_0082db60* self, void* /*edx*/)
{
    return self->field_008c;
}

HOOK(0x0040C9D0, VirtualProcessor_GetExecutionResourceId, "auto");
