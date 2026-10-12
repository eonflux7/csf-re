#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, sub_519EC0, 0x00519EC0, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, sub_51A3E0, 0x0051A3E0, (void* self, void* edx, int a2, int a3));

// Looks up the record at index recordIndex in this object's table (table pointer at +0x04,
// record array at +0x04 of the table, 96-byte records), fills it via sub_519EC0 with the
// embedded sub-object at +0x18, then forwards to sub_51A3E0.
static int __fastcall C_00840724_LookupRecordAndDispatch(C_00840724* self, void* /*edx*/, int recordIndex, int dispatchArg)
{
    int recordOut = 0;

    uint8_t* table = reinterpret_cast<uint8_t*>(self->field_0004);
    uint8_t* records = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, table, 4));
    uint32_t* record = reinterpret_cast<uint32_t*>(records + 96 * recordIndex);

    sub_519EC0(reinterpret_cast<uint8_t*>(self) + 0x18, nullptr, reinterpret_cast<int>(&recordOut), record);
    return sub_51A3E0(self, nullptr, recordIndex, dispatchArg);
}

HOOK(0x0051A6F0, C_00840724_LookupRecordAndDispatch, "auto");
