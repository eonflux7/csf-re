#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stores the description value at +0x3C and returns it.
static int __fastcall MenuExtrasControl_SetDescription(void* self, void* /*edx*/, int description) {
    CSF_AT(int, self, 0x3C) = description;
    return description;
}

HOOK(0x4D15F0, MenuExtrasControl_SetDescription, "auto");
