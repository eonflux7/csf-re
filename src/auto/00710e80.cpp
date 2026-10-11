#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Copies a 3-dword record (likely a vec3) from the sub-object at this->field_0008 + 0x28
// into the caller's buffer, then returns the destination pointer.
static uint32_t* __fastcall C_00863ba8_CopyVec3FromOffset28(C_00863ba8* self, void* /*edx*/, uint32_t* destination) {
    uint32_t* source = reinterpret_cast<uint32_t*>(self->field_0008 + 0x28);
    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
    return destination;
}

HOOK(0x710E80, C_00863ba8_CopyVec3FromOffset28, "auto");
