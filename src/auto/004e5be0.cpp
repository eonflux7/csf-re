#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: sub_4E58E0 returns an index into the entry table, or -1 when not found.
CSF_FUNC(int, __fastcall, sub_4E58E0, 0x004E58E0, (void* self, void* edx, int a2));

// Returns 1 when the entry located for a2 has its state dword equal to 1, otherwise 0.
// Entries in the table at field_0020 are 20 bytes each; the state is the first dword.
static char __fastcall C_0083e9d0_IsIndexedEntryStateOne(C_0083e9d0* self, void* /*edx*/, int a2)
{
    int entryIndex = sub_4E58E0(self, nullptr, a2);
    if (entryIndex == -1)
    {
        return 0;
    }

    uint8_t* entryTable = reinterpret_cast<uint8_t*>(self->field_0020);
    int entryState = *reinterpret_cast<int*>(entryTable + 20 * entryIndex);
    if (entryState != 1)
    {
        return 0;
    }
    return 1;
}

HOOK(0x004E5BE0, C_0083e9d0_IsIndexedEntryStateOne, "auto");
