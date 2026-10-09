#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns the dword at index `index` of the table pointed to by the pointer field at +0x74.
static int __fastcall MissionScript_LookupEntryIndex(void* self, void* /*edx*/, int index) {
    int* table = CSF_AT(int*, self, 0x74);
    return table[index];
}

HOOK(0x46F390, MissionScript_LookupEntryIndex, "auto");
