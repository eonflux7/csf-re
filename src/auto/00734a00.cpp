#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the address of the float field at offset 0xEC of this object.
static float* __fastcall C_008670f0_GetField00EC(C_008670f0* self, void* /*edx*/)
{
    return &self->field_00ec;
}

HOOK(0x00734A00, C_008670f0_GetField00EC, "auto");
