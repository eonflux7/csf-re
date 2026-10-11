#include <cstdint>
#include <cstring>

#include "hook.h"

// Copies the 7-dword record embedded at a1+0x2C, then returns a2 when the
// record's third dword (index 2, at +0x8 of the copy) is >= a2 as unsigned
// values; otherwise returns 0.
static int __cdecl AudioFxDict_ClampToSlot2Limit(int a1, unsigned int a2) {
    uint32_t record[7];
    const uint8_t* src = reinterpret_cast<const uint8_t*>(static_cast<uintptr_t>(a1)) + 0x2C;
    std::memcpy(record, src, sizeof(record));

    const uint32_t slot2 = record[2];
    return slot2 >= a2 ? static_cast<int>(a2) : 0;
}

HOOK(0x007128F0, AudioFxDict_ClampToSlot2Limit, "auto");
