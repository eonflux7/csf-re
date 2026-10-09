#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Reads the pointer stored at obj+0x04 (the sub-record), and the byte flag at sub+0x01.
static uint32_t GetSubRecordAddress(uint32_t objectAddress) {
    return CSF_AT(uint32_t, reinterpret_cast<void*>(objectAddress), 0x04);
}

static uint8_t GetSubRecordFlag(uint32_t subRecordAddress) {
    return CSF_AT(uint8_t, reinterpret_cast<void*>(subRecordAddress), 0x01);
}

static void SetSubRecordFlag(uint32_t subRecordAddress, uint8_t value) {
    CSF_AT(uint8_t, reinterpret_cast<void*>(subRecordAddress), 0x01) = value;
}

// Sets the owner's flag to 1 when either of the two paired objects has its flag set,
// otherwise clears it to 0. Returns 1 or 0 accordingly.
// flagPair[0] and flagPair[1] are pointers to the two objects being checked.
static int __stdcall UpdateOwnerFlagFromPairFlags(int ownerObject, int /*unused*/, uint32_t* flagPair) {
    uint32_t firstObjectAddress = flagPair[0];
    uint32_t firstSubRecord = GetSubRecordAddress(firstObjectAddress);
    if (GetSubRecordFlag(firstSubRecord) != 0) {
        uint32_t ownerSubRecord = GetSubRecordAddress(static_cast<uint32_t>(ownerObject));
        SetSubRecordFlag(ownerSubRecord, 1);
        return 1;
    }

    uint32_t secondObjectAddress = flagPair[1];
    uint32_t secondSubRecord = GetSubRecordAddress(secondObjectAddress);
    if (GetSubRecordFlag(secondSubRecord) != 0) {
        uint32_t ownerSubRecord = GetSubRecordAddress(static_cast<uint32_t>(ownerObject));
        SetSubRecordFlag(ownerSubRecord, 1);
        return 1;
    }

    uint32_t ownerSubRecord = GetSubRecordAddress(static_cast<uint32_t>(ownerObject));
    SetSubRecordFlag(ownerSubRecord, 0);
    return 0;
}

HOOK(0x0045AE70, UpdateOwnerFlagFromPairFlags, "auto");
