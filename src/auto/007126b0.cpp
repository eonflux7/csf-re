#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"

// Reads a float stored at offset 0x74 of the object pointed to by
// C_00863dd0::field_0004 and returns it as a double (x87 `fld dword ptr`).
static double __fastcall C_00863dd0_GetFloatAt0074(C_00863dd0* self, void* /*edx*/)
{
    const auto* owner = reinterpret_cast<const uint8_t*>(static_cast<uintptr_t>(self->field_0004));
    return *reinterpret_cast<const float*>(owner + 0x74);
}

HOOK(0x007126B0, C_00863dd0_GetFloatAt0074, "auto");
