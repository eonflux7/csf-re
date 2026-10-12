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

CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __cdecl, _free, 0x007B17CC, (void* Block));

// Frees every entry in the table (and each entry's owned buffer), then the table itself.
// Entries are tagged: byte 0 = owns a buffer; byte 0x28 = buffer came from the CRT allocator.
static int __fastcall ReleaseEntryTable(S_004713e0* self, void* /*edx*/)
{
    SetAllocationMode(5);

    for (uint32_t i = 0; i < self->field_0034; ++i) {
        uint32_t entry = reinterpret_cast<uint32_t*>(self->field_002c)[i];
        if (entry) {
            uint8_t* entryBytes = reinterpret_cast<uint8_t*>(entry);
            if (CSF_AT(uint8_t, entryBytes, 0)) {
                if (CSF_AT(uint8_t, entryBytes, 0x28)) {
                    _free(reinterpret_cast<void*>(CSF_AT(uint32_t, entryBytes, 4)));
                } else if (CSF_AT(uint32_t, entryBytes, 4)) {
                    Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, entryBytes, 4)));
                    CSF_AT(uint32_t, entryBytes, 4) = 0;
                }
            }
            Mem_Free(reinterpret_cast<void*>(entry));
            reinterpret_cast<uint32_t*>(self->field_002c)[i] = 0;
        }
    }

    uint32_t table = self->field_002c;
    self->field_0030 = 0;
    self->field_0034 = 0;
    if (table) {
        Mem_Free(reinterpret_cast<void*>(table));
        self->field_002c = 0;
    }

    return SetAllocationMode(4);
}

HOOK(0x0069B280, ReleaseEntryTable, "auto");
