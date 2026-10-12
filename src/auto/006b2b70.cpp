#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"

// Returns the dword at index `index` of the table at +0x18, or 0 when index >= the
// 16-bit count at +0x1C (unsigned compare, count zero-extended).
static int __fastcall C_00860288_GetEntryOrZero(C_00860288* self, void* /*edx*/, uint32_t index) {
    const uint32_t entryCount = self->field_001c;
    if (index >= entryCount) {
        return 0;
    }
    const auto* entries = reinterpret_cast<const int32_t*>(self->field_0018);
    return entries[index];
}

HOOK(0x6B2B70, C_00860288_GetEntryOrZero, "auto");
