#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the component type of a mission-script entry. When the byte at +0x0B is 0xFF,
// the value comes from the dword that the pointer at +0x00 points to. Otherwise it is the
// sign-extended byte at +0x0A.
static int __fastcall MissionScript_GetComponentType(S_00692b80* self, void* /*edx*/) {
    if (self->field_000b == 0xFF) {
        return static_cast<int>(*reinterpret_cast<const uint32_t*>(self->field_0000));
    }
    return CSF_AT(int8_t, self, 0x0A);
}

HOOK(0x006927F0, MissionScript_GetComponentType, "auto");
