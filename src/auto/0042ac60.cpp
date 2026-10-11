#include <cstdint>
#include <cstddef>
#include "hook.h"

// Scans a table of 0x20-slot entries (20-byte records) for the (a2, a3) pair
// and returns a pointer to the record at index a1 after updating it.
static uint32_t* __cdecl MenuControls_BuildConfigEntries(int a1, int a2, int a3, int a4) {
    uint32_t* table = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a4));
    const uint32_t key1 = static_cast<uint32_t>(a2);
    const uint32_t key2 = static_cast<uint32_t>(a3);

    unsigned int slotCount = 0;
    uint32_t* scan = table + 6; // a4 + 24

    for (; scan[-5] != key2 || scan[-6] != key1; scan += 20) {
        if (scan[0] == key2 && scan[-1] == key1) {
            ++slotCount;
            break;
        }
        if (scan[5] == key2 && scan[4] == key1) {
            slotCount += 2;
            break;
        }
        if (scan[10] == key2 && scan[9] == key1) {
            slotCount += 3;
            break;
        }
        slotCount += 4;
        if (slotCount >= 0x20) {
            uint32_t* result = table + 5 * a1;
            result[0] = key1;
            result[1] = key2;
            return result;
        }
    }

    if (slotCount < 0x20) {
        uint32_t* source = table + 5 * a1;
        uint32_t* destination = table + 5 * slotCount;
        destination[0] = source[0];
        destination[1] = source[1];
    }

    uint32_t* result = table + 5 * a1;
    result[0] = key1;
    result[1] = key2;
    return result;
}

HOOK(0x0042AC60, MenuControls_BuildConfigEntries, "auto");
