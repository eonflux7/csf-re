#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the index of the record matching a2 inside the table at this->field_0010, or -1.
CSF_FUNC(int, __fastcall, S_0044bc20_FindRecordIndex, 0x0044BC20, (void* self, void* edx, int a2));
// Moves a block of records (100-byte entries) to the destination (cdecl, 3 declared args).
CSF_FUNC(char*, __cdecl, S_0044bc20_CopyRecordBlock, 0x0044C150, (int a1, int a2, char* a3));
// Shifts the tail of the table down over a removed record (cdecl, 2 declared args).
CSF_FUNC(int, __cdecl, S_0044bc20_CompactRecordTail, 0x0044BFC0, (int a1, int a2));

// Removes the record at index found by FindRecordIndex(a2) from the table and decrements the count.
// Returns the new count, or -1 if the record was not found.
static int __fastcall S_0044bc20_RemoveRecordAt(S_0044bc20* self, void* /*edx*/, int a2)
{
    int result = S_0044bc20_FindRecordIndex(self, nullptr, a2);
    if (result != -1)
    {
        (void)CSF_VTBL(0x0044BC20);  // vtable reference listed in the packet (S_0044bc20 vtable)

        uint32_t recordAddr = self->field_0010 + 100 * static_cast<uint32_t>(result);
        S_0044bc20_CopyRecordBlock(static_cast<int>(recordAddr + 100),
                                   static_cast<int>(self->field_0014),
                                   reinterpret_cast<char*>(recordAddr));

        S_0044bc20_CompactRecordTail(static_cast<int>(self->field_0014) - 100,
                                     static_cast<int>(self->field_0014));

        result = static_cast<int>(self->field_0014) - 100;
        self->field_0014 = static_cast<uint32_t>(result);
    }
    return result;
}

HOOK(0x0044C5A0, S_0044bc20_RemoveRecordAt, "auto");
