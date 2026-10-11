#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Copies a 3-dword record (likely a vec3) from the sub-object at this->field_0004 + 0x58
// into the caller's buffer, then returns the destination pointer.
static uint32_t* __fastcall C_00863dd0_CopyVec3FromOffset88(C_00863dd0* self, void* /*edx*/, uint32_t* destination) {
    uint32_t* source = reinterpret_cast<uint32_t*>(self->field_0004 + 0x58);
    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
    return destination;
}

HOOK(0x712A20, C_00863dd0_CopyVec3FromOffset88, "auto");
