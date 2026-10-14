#include <cstdint>
#include "globals.h"
#include "hook.h"

static char* __cdecl Assign(char* first, char* last, const void* value)
{
    for (; first != last; first += 28) {
        // rep movsd copies in increasing dword order, even for overlapping inputs.
        for (unsigned i = 0; i != 28; i += 4)
            CSF_AT(uint32_t, first, i) = CSF_AT(uint32_t, value, i);
    }
    return first;
}
HOOK(0x006DCFF0, Assign, "stl");
