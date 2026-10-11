#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns bit 6 of the flags dword stored in the object referenced by
// self->+0x04, but only when self->+0x1C (a pointer) is non-null.
static char __fastcall C_00861e98_IsFlagBit6Set(C_00861e98* self, void* /*edx*/) {
    if (CSF_AT(uint32_t, self, 0x1C) == 0) {
        return 0;
    }
    uint8_t* linkedObject = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x04));
    uint32_t flags = CSF_AT(uint32_t, linkedObject, 0x14);
    return static_cast<char>((flags >> 6) & 1u);
}

HOOK(0x006E9820, C_00861e98_IsFlagBit6Set, "auto");
