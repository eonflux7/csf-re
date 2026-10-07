#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the dword at offset 0x74 of the object referenced by field_0040.
static int __fastcall C_008319c0_GetSubObjectField74(C_008319c0* self, void* /*edx*/)
{
    uint32_t* subObject = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_0040));
    return static_cast<int>(subObject[0x74 / 4]);
}

HOOK(0x004760A0, C_008319c0_GetSubObjectField74, "auto");
