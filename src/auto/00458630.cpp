#include <cstdint>
#include "globals.h"
#include "hook.h"

static uint32_t* __cdecl CopyBackward(uint32_t* first, uint32_t* last, uint32_t* dest) {
    while (last != first) {
        last -= 10;
        dest -= 10;
        for (unsigned i = 0; i != 10; ++i) dest[i] = last[i];
    }
    return dest;
}
HOOK(0x00458630, CopyBackward, "stl");
