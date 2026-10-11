#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Probes the object for five candidate types (13, 14, 11, 10, 9) in that order.
// The first one the virtual probe accepts is stored at out+8 and the state at
// out+4 becomes 3. Returns whether out+4 ended up different from -1.
static int __fastcall C_0083eb70_FindAvailableAction(C_0083eb70* self, void* /*edx*/, int a2)
{
    uint8_t* out = reinterpret_cast<uint8_t*>(a2);

    CSF_AT(int, out, 4) = -1;

    if (self->vSlot9C(a2 + 20, 13)) {
        CSF_AT(int, out, 8) = 13;
        CSF_AT(int, out, 4) = 3;
    } else if (self->vSlot9C(a2 + 20, 14)) {
        CSF_AT(int, out, 8) = 14;
        CSF_AT(int, out, 4) = 3;
    } else if (self->vSlot9C(a2 + 20, 11)) {
        CSF_AT(int, out, 8) = 11;
        CSF_AT(int, out, 4) = 3;
    } else if (self->vSlot9C(a2 + 20, 10)) {
        CSF_AT(int, out, 8) = 10;
        CSF_AT(int, out, 4) = 3;
    } else if (self->vSlot9C(a2 + 20, 9)) {
        CSF_AT(int, out, 8) = 9;
        CSF_AT(int, out, 4) = 3;
    }

    return CSF_AT(int, out, 4) != -1;
}

HOOK(0x4E6E80, C_0083eb70_FindAvailableAction, "auto");
