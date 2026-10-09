#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies two dwords (+0x20, +0x24) from a load record into one of the slot
// table's 88-byte entries. The record's kind at +0x54 selects the target:
//   1 -> entry a2, bytes +0x28/+0x2C
//   2 -> current entry, bytes +0x34/+0x38
//   3 -> current entry, bytes +0x40/+0x44
// Any other kind writes nothing and returns the kind value itself.
static uint32_t* __fastcall CSceneLoader_ApplySlotRecord(C_0083f5d8* self, void* /*edx*/, int a2, uint32_t* a3) {
    uint32_t* result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, a3, 0x54)));

    if (result == reinterpret_cast<uint32_t*>(1)) {
        uint8_t* slotTable = CSF_AT(uint8_t*, self, 0x30);
        result = reinterpret_cast<uint32_t*>(slotTable + a2 * 0x58);
        CSF_AT(uint32_t, result, 0x28) = CSF_AT(uint32_t, a3, 0x20);
        CSF_AT(uint32_t, result, 0x2C) = CSF_AT(uint32_t, a3, 0x24);
    }
    else if (result == reinterpret_cast<uint32_t*>(2)) {
        uint8_t* slotTable = CSF_AT(uint8_t*, self, 0x30);
        int currentIndex = CSF_AT(int, self, 0x0C);
        result = reinterpret_cast<uint32_t*>(slotTable + currentIndex * 0x58);
        CSF_AT(uint32_t, result, 0x34) = CSF_AT(uint32_t, a3, 0x20);
        CSF_AT(uint32_t, result, 0x38) = CSF_AT(uint32_t, a3, 0x24);
    }
    else if (result == reinterpret_cast<uint32_t*>(3)) {
        uint8_t* slotTable = CSF_AT(uint8_t*, self, 0x30);
        int currentIndex = CSF_AT(int, self, 0x0C);
        result = reinterpret_cast<uint32_t*>(slotTable + currentIndex * 0x58);
        CSF_AT(uint32_t, result, 0x40) = CSF_AT(uint32_t, a3, 0x20);
        CSF_AT(uint32_t, result, 0x44) = CSF_AT(uint32_t, a3, 0x24);
    }
    return result;
}

HOOK(0x005003A0, CSceneLoader_ApplySlotRecord, "auto");
