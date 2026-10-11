#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee: copies the trailing sub-record (source +0x1C) into the destination's +0x1C block.
CSF_FUNC(int, __fastcall, CopyWaypointTailRecord, 0x0075C090, (void* self, void* edx, int a2));

static uint32_t* __cdecl CopyWaypointRecordRange(uint32_t* first, uint32_t* last, uint32_t* source)
{
    uint32_t* cursor = first;
    while (cursor != last)
    {
        cursor[0] = source[0];
        cursor[1] = source[1];
        cursor[2] = source[2];
        cursor[3] = source[3];
        cursor[4] = source[4];
        cursor[5] = source[5];
        cursor[6] = source[6];
        CopyWaypointTailRecord(cursor + 7, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(source + 7)));
        cursor += 11;
    }
    return last;
}

HOOK(0x0075C3C0, CopyWaypointRecordRange, "auto");
