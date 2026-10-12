#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t* __fastcall BddTables_AssociateProviderSector(
    void* self, void* /*edx*/, int index, uint32_t sector)
{
    uint32_t* table = CSF_AT(uint32_t*, self, 0x20);
    uint32_t* slot = &table[index];
    *slot = sector;
    return slot;
}

HOOK(0x0051AF30, BddTables_AssociateProviderSector, "auto");
