#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4E58E0, 0x004E58E0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, sub_4E6690, 0x004E6690, (void* self, void* edx, uint32_t* a2));

// Looks up the record keyed by a2[2] in the table; if absent, inserts a2 as a new entry.
// Copies fields a2[0], a2[1], a2[3], a2[4] into the entry, then raises the entry's
// int at +4 to a3 if it is smaller (signed compare).
static unsigned int __fastcall C_0083e9d0_MergeRecordIntoTable(C_0083e9d0* self, void* /*edx*/, uint32_t* a2, int a3)
{
    unsigned int result = static_cast<unsigned int>(sub_4E58E0(self, nullptr, (int)(a2 + 2)));
    if (result == static_cast<unsigned int>(-1))
    {
        result = sub_4E6690(self, nullptr, a2);
    }
    else
    {
        uint32_t entryAddress = self->field_0020 + 20u * result;
        uint32_t* entry = reinterpret_cast<uint32_t*>(entryAddress);
        entry[0] = a2[0];
        entry[1] = a2[1];
        entry[3] = a2[3];
        entry[4] = a2[4];
    }

    // Re-read the table base after the call: the callee may have changed it.
    uint32_t slotAddress = self->field_0020 + 20u * result + 4u;
    int* maxSlot = reinterpret_cast<int*>(slotAddress);
    if (*maxSlot < a3)
        *maxSlot = a3;
    return result;
}

HOOK(0x004E6750, C_0083e9d0_MergeRecordIntoTable, "auto");
