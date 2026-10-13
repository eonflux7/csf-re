#include <cstring>

#include "hook.h"
#include "types/auto_classes.h"

// Stores the raw 32-bit value a2 into the dword at +0x10 (declared as float) and returns it.
static int __fastcall C_00863430_SetRawField10(C_00863430* self, void* /*edx*/, int a2) {
    std::memcpy(&self->field_0010, &a2, sizeof(int));
    return a2;
}

HOOK(0x6C9FF0, C_00863430_SetRawField10, "auto");
