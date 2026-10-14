#include <cstdint>
#include "globals.h"
#include "hook.h"

static char* __cdecl CopyBackward(char* first, char* last, char* destination)
{
    while (last != first) {
        last -= 28;
        destination -= 28;
        // Elements move backwards; the seven dwords within each move forwards.
        for (unsigned i = 0; i != 28; i += 4)
            CSF_AT(uint32_t, destination, i) = CSF_AT(uint32_t, last, i);
    }
    return destination;
}
HOOK(0x006DE100, CopyBackward, "stl");
