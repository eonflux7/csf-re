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

// Scans the object's pointer table (count at +0x10, array at +0x08) and returns the
// first non-null entry whose object has the value a2 stored at +0x0C. Returns 0 if none.
static int __fastcall C_00867078_CanConfigureEffect(C_00867078* self, void* /*edx*/, int a2) {
    uint32_t entryCount = self->field_0010;
    if (entryCount == 0) {
        return 0;
    }

    uint8_t* slotArray = reinterpret_cast<uint8_t*>(self->field_0008);
    uint32_t index = 0;
    for (;;) {
        uint32_t entry = *reinterpret_cast<uint32_t*>(slotArray + 4 * index);
        if (entry != 0) {
            uint8_t* entryObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entry));
            if (CSF_AT(int, entryObject, 0xC) == a2) {
                return static_cast<int>(entry);
            }
        }
        if (++index >= entryCount) {
            return 0;
        }
    }
}

HOOK(0x007319C0, C_00867078_CanConfigureEffect, "auto");
