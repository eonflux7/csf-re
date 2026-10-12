#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the result of a virtual query on the child object at +0x40.
// If the query reports true (low byte of eax nonzero), sets flag 0x80 at +0x2E0.
static int __fastcall RefreshFlag80FromChildQuery(void* self, void* /*edx*/) {
    void* child = CSF_AT(void*, self, 0x40);
    // vtable slot 0x108 / 4 = 66
    int result = ((int (__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[66])(child, nullptr);
    if ((uint8_t)result) {
        CSF_AT(uint32_t, self, 0x2E0) |= 0x80u;
    }
    return result;
}

HOOK(0x004951F0, RefreshFlag80FromChildQuery, "auto");
