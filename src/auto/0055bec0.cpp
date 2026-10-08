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

// Linear search over a uint32_t array stored in the object.
// Count is at +0x10, array pointer at +0x08. Returns true if value is present.
static bool __fastcall C_008453a4_ContainsValue(C_008453a4* self, void* /*edx*/, int value) {
    const uint32_t entryCount = self->field_0010;
    if (entryCount == 0) {
        return false;
    }

    const uint32_t* entries = reinterpret_cast<const uint32_t*>(self->field_0008);
    for (uint32_t index = 0; index < entryCount; ++index) {
        if (entries[index] == static_cast<uint32_t>(value)) {
            return true;
        }
    }
    return false;
}

HOOK(0x0055BEC0, C_008453a4_ContainsValue, "auto");
