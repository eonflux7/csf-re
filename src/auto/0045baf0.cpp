#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

static bool __stdcall MatchesTaggedOrPlainKey(int keyArg, int recordArg) {
    const uint32_t kFlagBit = 0x8000000u;
    const uint32_t kFlagMask = 0xF7FFFFFFu;

    int32_t recordTag = CSF_AT(int32_t, recordArg, 0x4);
    if ((static_cast<uint32_t>(keyArg) & kFlagBit) == 0) {
        return keyArg == recordTag;
    }
    if (recordTag != 1) {
        return false;
    }
    int32_t recordSubKey = CSF_AT(int32_t, recordArg, 0x24);
    return recordSubKey == static_cast<int32_t>(static_cast<uint32_t>(keyArg) & kFlagMask);
}

HOOK(0x45BAF0, MatchesTaggedOrPlainKey, "auto");
