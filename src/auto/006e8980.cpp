#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword at +4 of the object pointed to by the field at +0x14,
// provided the field at +0x1C is non-null and the +0x14 pointer is non-null.
static int __fastcall C_00861e98_GetLinkedPayloadValue(C_00861e98* self, void* /*edx*/) {
    if (CSF_AT(uint32_t, self, 0x1c) != 0) {
        uint32_t linkedPtr = CSF_AT(uint32_t, self, 0x14);
        if (linkedPtr != 0) {
            uint8_t* linked = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedPtr));
            return CSF_AT(int, linked, 4);
        }
    }
    return 0;
}

HOOK(0x6E8980, C_00861e98_GetLinkedPayloadValue, "auto");
