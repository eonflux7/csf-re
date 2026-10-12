#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, BddTables_FillRecordEntry, 0x00519EC0, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, C_00869cd0_GetRecordAddress, 0x00755BB0, (C_00869cd0* self, void* /*edx*/, int index));

// Fills a local out-buffer from the record at table[a2] (80-byte stride) and returns the record address for a2.
static int __fastcall C_008406d0_FillRecordThenGetAddress(C_008406d0* self, void* /*edx*/, int a2)
{
    // Out-parameter buffer on the stack (original: [ebp-8], 8 bytes of room).
    alignas(8) uint8_t outBytes[8];

    // Table base is stored in the dword at +4 of the object pointed to by self->field_0004.
    uint8_t* sibling = reinterpret_cast<uint8_t*>(self->field_0004);
    uint32_t tableBase = CSF_AT(uint32_t, sibling, 0x4);
    uint32_t recordAddress = tableBase + static_cast<uint32_t>(a2 * 80);

    // Embedded sub-object at +0x58 of this.
    void* embedded = reinterpret_cast<uint8_t*>(self) + 0x58;
    BddTables_FillRecordEntry(embedded, nullptr,
                              reinterpret_cast<int>(outBytes),
                              reinterpret_cast<uint32_t*>(recordAddress));

    return C_00869cd0_GetRecordAddress(reinterpret_cast<C_00869cd0*>(self), nullptr, a2);
}

HOOK(0x0051B8A0, C_008406d0_FillRecordThenGetAddress, "auto");
