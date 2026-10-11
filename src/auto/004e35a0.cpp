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

// Returns true when any 12-byte entry in the table at self+0x34 (count at self+0x3C)
// has the key (dword at +4) equal to the key stored at argument+4.
static bool __fastcall TableContainsKey(uint32_t* self, void* /*edx*/, int a2) {
    uint32_t entryCount = CSF_AT(uint32_t, self, 0x3C);
    bool found = false;

    if (entryCount != 0) {
        uint32_t tableBase = CSF_AT(uint32_t, self, 0x34);
        uint8_t* entry = reinterpret_cast<uint8_t*>(tableBase) + 4;
        uint32_t searchKey = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a2), 4);

        for (uint32_t index = 0; index < entryCount; ++index) {
            if (CSF_AT(uint32_t, entry, 0) == searchKey) {
                found = true;
                break;
            }
            entry += 12;
        }
    }

    return found;
}

HOOK(0x4E35A0, TableContainsKey, "auto");
