#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies a 0x24-byte DDL header block from the source record into the object
// at offsets 0x44..0x64, field by field. Returns the last dword copied.
static int __fastcall NetCogibleDDL_CopyRecord(void* self, void* /*edx*/, void* source)
{
    CSF_AT(uint16_t, self, 0x44) = CSF_AT(uint16_t, source, 0x00);
    CSF_AT(uint16_t, self, 0x46) = CSF_AT(uint16_t, source, 0x02);
    CSF_AT(uint32_t, self, 0x48) = CSF_AT(uint32_t, source, 0x04);
    CSF_AT(uint32_t, self, 0x4C) = CSF_AT(uint32_t, source, 0x08);
    CSF_AT(uint32_t, self, 0x50) = CSF_AT(uint32_t, source, 0x0C);
    CSF_AT(uint32_t, self, 0x54) = CSF_AT(uint32_t, source, 0x10);
    CSF_AT(uint32_t, self, 0x58) = CSF_AT(uint32_t, source, 0x14);
    CSF_AT(uint32_t, self, 0x5C) = CSF_AT(uint32_t, source, 0x18);
    CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, source, 0x1C);

    int result = CSF_AT(int, source, 0x20);
    CSF_AT(int, self, 0x64) = result;
    return result;
}

HOOK(0x7403C0, NetCogibleDDL_CopyRecord, "auto");
