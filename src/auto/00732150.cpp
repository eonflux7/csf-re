#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of child slot `index` in a 16-byte-stride array whose base is field_000c.
static int __fastcall C_0086709c_GetChildAtIndex(C_0086709c* self, void* /*edx*/, int index) {
    uint32_t base = self->field_000c;
    uint32_t offset = static_cast<uint32_t>(index) << 4;
    return static_cast<int>(base + offset);
}

HOOK(0x732150, C_0086709c_GetChildAtIndex, "auto");
