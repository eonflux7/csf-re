#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Destroys the nodes in the range [begin, end) of the graph buffer owned by 'owner'.
CSF_FUNC(int, __cdecl, WaypointGraph_DestroyRange, 0x0075D0E0, (int a1, int a2, int a3, int a4));
// Frees a game-heap block (debug allocation tracker).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the dynamically allocated node buffer owned by this graph parser record
// (buffer pointer at +0x04, end at +0x08, extra pointer at +0x0C). Runs the range
// destroyer over the buffer, frees the storage with the game heap and clears the
// three fields. Returns the low byte of the last value held in the buffer slot.
static char __fastcall WaypointGraph_ReleaseBuffer(S_0051aa70* self, void* /*edx*/)
{
    int v2 = static_cast<int>(self->field_0004);
    if (v2 != 0)
    {
        // The fourth argument was an uninitialized stack local in the original; pass 0.
        WaypointGraph_DestroyRange(v2, static_cast<int>(self->field_0008),
                                   reinterpret_cast<int>(self), 0);

        // The range destroyer may have modified the record, so reload the field.
        v2 = static_cast<int>(self->field_0004);
        if (v2 != 0)
        {
            v2 = Mem_FreeDebug(reinterpret_cast<void*>(v2));
        }
    }

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    return static_cast<char>(v2);
}

HOOK(0x0075D3B0, WaypointGraph_ReleaseBuffer, "auto");
