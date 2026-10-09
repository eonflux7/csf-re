#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address.
CSF_FUNC(int, __cdecl, sub_500D60, 0x00500D60, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Clears the per-scene flag at +0x50 of every 88-byte scene entry, then releases the
// entry buffer (header at this+0x2C: +4 begin, +8 end, +0xC capacity end) and resets it.
static int __fastcall CSceneLoaderBase_ClearSceneEntriesAndReleaseList(C_0083f578* self, void* /*edx*/) {
    uint8_t* bufferHeader = reinterpret_cast<uint8_t*>(self) + 0x2C;

    uint32_t entryIndex = 0;
    for (int entryOffset = 0;; entryOffset += 88) {
        uint32_t beginAddress = CSF_AT(uint32_t, bufferHeader, 4);
        if (beginAddress == 0) {
            break;
        }
        int32_t byteSpan = static_cast<int32_t>(CSF_AT(uint32_t, bufferHeader, 8) - beginAddress);
        int32_t entryCount = byteSpan / 88;
        if (entryIndex >= static_cast<uint32_t>(entryCount)) {
            break;
        }
        uint8_t* entries = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x30));
        CSF_AT(int32_t, entries, entryOffset + 0x50) = 0;
        ++entryIndex;
    }

    uint32_t beginAddress = CSF_AT(uint32_t, bufferHeader, 4);
    if (beginAddress != 0) {
        uint32_t endAddress = CSF_AT(uint32_t, bufferHeader, 8);
        sub_500D60(static_cast<int>(beginAddress),
                   static_cast<int>(endAddress),
                   static_cast<int>(reinterpret_cast<uintptr_t>(bufferHeader)),
                   0);
        beginAddress = CSF_AT(uint32_t, bufferHeader, 4);
        if (beginAddress != 0) {
            Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(beginAddress)));
        }
    }

    CSF_AT(uint32_t, bufferHeader, 4) = 0;
    CSF_AT(uint32_t, bufferHeader, 8) = 0;
    CSF_AT(uint32_t, bufferHeader, 0xC) = 0;
    return 0;
}

HOOK(0x00503710, CSceneLoaderBase_ClearSceneEntriesAndReleaseList, "auto");
