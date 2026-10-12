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

// Returns 1 when the 16-bit index stored at entry+0x20 is set in the dword
// bitmask table at group+0x60, where entry is this->field_0004 and group is
// entry+0x24. Returns 0 when entry or group is null.
static int __fastcall C_008630e0_IsIndexBitSetInGroupMask(C_008630e0* self, void* /*edx*/) {
    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0004));
    if (entry == nullptr) {
        return 0;
    }

    uint32_t groupAddress = CSF_AT(uint32_t, entry, 0x24);
    if (groupAddress == 0) {
        return 0;
    }

    uint16_t bitIndex = CSF_AT(uint16_t, entry, 0x20);
    uint8_t* group = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(groupAddress));
    uint32_t maskTableAddress = CSF_AT(uint32_t, group, 0x60);

    uint32_t* maskTable = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(maskTableAddress));
    uint32_t maskWord = maskTable[bitIndex >> 5];

    return (maskWord & (1u << (bitIndex & 0x1F))) != 0 ? 1 : 0;
}

HOOK(0x6FF7A0, C_008630e0_IsIndexBitSetInGroupMask, "auto");
