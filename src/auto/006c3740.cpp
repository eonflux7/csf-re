#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Writes a 3-dword triplet at entry `index` (stride 36) of the table at this+4,
// and, if a 4-byte source is given, a packed big-endian-style dword at entry+0x18.
// Returns 9 * index when `packed` is non-null, otherwise 0.
static int __fastcall SetEntryVec3AndPackedRgba(void* self, void* /*edx*/,
                                                int index,
                                                const uint32_t* triplet,
                                                const uint8_t* packed)
{
    if (triplet) {
        const int entryOffset = 36 * index;
        *reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x4) + entryOffset + 0x0) = triplet[0];
        *reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x4) + entryOffset + 0x4) = triplet[1];
        *reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x4) + entryOffset + 0x8) = triplet[2];
    }

    if (packed) {
        const uint32_t byte0 = packed[0];
        const uint32_t byte1 = packed[1];
        const uint32_t byte2 = packed[2];
        const uint32_t byte3 = packed[3];
        const uint32_t value = (((byte3 << 8) | byte0) << 16) | (byte1 << 8) | byte2;
        *reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x4) + 36 * index + 0x18) = value;
        return 9 * index;
    }
    return 0;
}

HOOK(0x6C3740, SetEntryVec3AndPackedRgba, "auto");
