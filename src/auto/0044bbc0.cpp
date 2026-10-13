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

// Scans the 100-byte records between begin (+0x10) and end (+0x14) of this object
// for a record whose tag dword is 2 and whose key at +0x14 equals the key at a2+4.
static char __fastcall TCharTask_RunAttackStep(void* self, void* /*edx*/, int a2) {
    uint32_t recordCount = 0;
    uint32_t beginAddress = CSF_AT(uint32_t, self, 0x10);
    if (beginAddress) {
        int32_t byteSpan = (int32_t)(CSF_AT(uint32_t, self, 0x14) - beginAddress);
        recordCount = (uint32_t)(byteSpan / 100);
    }
    if (recordCount == 0) {
        return 0;
    }

    uint8_t* record = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x10));
    for (uint32_t recordIndex = 0; recordIndex < recordCount; ++recordIndex) {
        if (CSF_AT(uint32_t, record, 0) == 2 &&
            CSF_AT(uint32_t, record, 0x14) == CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a2), 4)) {
            return 1;
        }
        record += 0x64;
    }
    return 0;
}

HOOK(0x0044BBC0, TCharTask_RunAttackStep, "auto");
