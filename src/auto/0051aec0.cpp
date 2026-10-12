#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

static int __fastcall BddTables_GetProviderStateIndex(void* self, void* /*edx*/, int index) {
    int* table = CSF_AT(int*, self, 0x20);
    return table[index];
}

HOOK(0x51AEC0, BddTables_GetProviderStateIndex, "auto");
