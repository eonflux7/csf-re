#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns bit 9 of the dword at +0x04: the pin-pointer flag of the DName object.
static int __fastcall DName_IsPinPtr(void* self, void* /*edx*/) {
    uint32_t flags = CSF_AT(uint32_t, self, 0x4);
    return static_cast<int>((flags >> 9) & 1u);
}

HOOK(0x6B81B0, DName_IsPinPtr, "auto");
