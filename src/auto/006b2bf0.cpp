#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword at index `index` of the table at +0x20, or 0 when index >= the
// 16-bit count at +0x24 (unsigned compare, count zero-extended).
static int __fastcall C_00860288_GetSecondaryEntryOrZero(C_00860288* self, void* /*edx*/, uint32_t index) {
    const uint32_t entryCount = static_cast<uint32_t>(self->field_0024);
    if (index >= entryCount) {
        return 0;
    }
    const auto* entries = reinterpret_cast<const int32_t*>(self->field_0020);
    return entries[index];
}

HOOK(0x6B2BF0, C_00860288_GetSecondaryEntryOrZero, "auto");
