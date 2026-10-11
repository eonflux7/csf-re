#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the 32-bit slot value at this + 0x2C + index * 16.
// Original: shl eax, 4 / mov eax, [eax + ecx + 0x2C]; ret 4
static int __fastcall C_00860bb8_GetIndexedSlotValue(C_00860bb8* self, void* /*edx*/, int index) {
    return CSF_AT(int, self, 0x2C + index * 16);
}

HOOK(0x006CB870, C_00860bb8_GetIndexedSlotValue, "auto");
