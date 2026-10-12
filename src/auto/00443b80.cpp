#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: __thiscall(this, a2) with ret 4, bound as the packet specifies.
CSF_FUNC(int, __fastcall, sub_443A00, 0x00443A00, (void* self, void* edx, int a2));

// Calls the lookup at 0x443A00 with the same `this` and argument, then clears
// the first byte of the 40-byte entry at index `result` inside the table at
// this->field_000c. Returns the index.
static int __fastcall S_00443a00_ClearEntryByteForIndex(S_00443a00* self, void* /*edx*/, int a2)
{
    int result = sub_443A00(self, nullptr, a2);

    uint8_t* entryTable = reinterpret_cast<uint8_t*>(self->field_000c);
    entryTable[40 * result] = 0;

    return result;
}

HOOK(0x00443B80, S_00443a00_ClearEntryByteForIndex, "auto");
