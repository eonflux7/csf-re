#include <cstdint>
#include "globals.h"
#include "hook.h"

static uint32_t* __cdecl Assign(uint32_t* first, uint32_t* last, const uint32_t* value) {
    while (first != last) {
        // REP MOVSD copies ten dwords in forward order, including for aliases.
        for (unsigned i = 0; i != 10; ++i) first[i] = value[i];
        first += 10;
    }
    return first;
}
HOOK(0x00458360, Assign, "stl");
