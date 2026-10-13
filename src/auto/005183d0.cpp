#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the 4-character protocol tag (packed as 'I''P''n''n' in little-endian
// order) that matches the variant code reported by the object's virtual slot 0x50.
// Unknown codes yield -1.
static int __fastcall C_0083f630_GetProtocolTag(C_0083f630* self, void* /*edx*/)
{
    // Slot 0x50 / 4 == 20. The packet's typed wrapper (vSlot0) is for slot 0x0,
    // so the slot is called directly through the vtable.
    using GetVariantCodeFn = int(__fastcall*)(void* self, void* edx);
    GetVariantCodeFn getVariantCode =
        reinterpret_cast<GetVariantCodeFn>(CSF_AT(void**, self, 0)[20]);

    int variantCode = getVariantCode(self, nullptr);

    switch (variantCode) {
    case 0:  return 1229991984;
    case 1:  return 1229991985;
    case 2:  return 1229991986;
    case 3:  return 1229991987;
    case 4:  return 1229991988;
    case 5:  return 1229991989;
    case 6:  return 1229991990;
    case 7:  return 1229992241;
    case 8:  return 1229991991;
    case 10: return 1229991992;
    case 11: return 1229991993;
    case 12: return 1229992240;
    default: return -1;
    }
}

HOOK(0x005183D0, C_0083f630_GetProtocolTag, "auto");
