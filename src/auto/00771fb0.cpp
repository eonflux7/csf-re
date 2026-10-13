#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Looks up the entry by id; returns its index, or -1 when absent.
CSF_FUNC(int, __fastcall, FindEntryIndex, 0x00771B80, (void* self, void* edx, int a2));

// Appends the record at a3 to this list (callee not yet named).
CSF_FUNC(uint32_t*, __fastcall, sub_771F30, 0x00771F30, (void* self, void* edx, uint32_t* a3));

// Adds the 3-dword record a2 to the list only when none of its three ids is -1
// and no entry with id a2[1] is already present.
static void __fastcall S_00771b80_AddEntryIfValid(S_00771b80* self, void* /*edx*/, uint32_t* record)
{
    if (record[0] != 0xFFFFFFFFu &&
        record[1] != 0xFFFFFFFFu &&
        record[2] != 0xFFFFFFFFu &&
        FindEntryIndex(self, nullptr, static_cast<int>(record[1])) == -1)
    {
        sub_771F30(self, nullptr, record);
    }
}

HOOK(0x00771FB0, S_00771b80_AddEntryIfValid, "auto");
