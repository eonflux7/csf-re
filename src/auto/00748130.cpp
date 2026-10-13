#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Searches one of two -1 terminated int lists stored on the object.
// The list is at +0x350 when a2 == 1, otherwise at +0x358. The search
// stops at the -1 sentinel or after the byte count at +0x69 entries.
// Returns true if a3 is found within that range.
static bool __fastcall FindValueInSlotList(void* self, void* /*edx*/, char a2, int a3)
{
    int maxEntries = CSF_AT(int8_t, self, 0x69);

    int* list = (a2 == 1) ? CSF_AT(int*, self, 0x350)
                          : CSF_AT(int*, self, 0x358);
    if (list == nullptr)
        return false;

    int index = 0;
    int* entry = list;
    for (;;)
    {
        int value = *entry;
        if (value == -1)
            break;
        if (index >= maxEntries)
            break;
        if (a3 == value)
            return true;
        ++index;
        ++entry;
    }
    return false;
}

HOOK(0x748130, FindValueInSlotList, "auto");
