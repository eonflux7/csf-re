#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Game heap free (0x691110, cdecl, caller pops the argument).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees the two owned buffers (+0x350 and +0x358), clears their pointers and
// resets the associated counters and flags. Returns the value Mem_Free returned
// for the second buffer, or the (null) buffer pointer if it was not freed.
static void* __fastcall C_00868010_FreeBuffersAndReset(C_00868010* self, void* /*edx*/)
{
    if (self->field_0350)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0350));
        self->field_0350 = 0;
    }

    void* result = reinterpret_cast<void*>(self->field_0358);
    self->field_0354 = 0;
    self->field_0360 = 0;
    if (result)
    {
        result = reinterpret_cast<void*>(Mem_Free(result));
        self->field_0358 = 0;
    }
    self->field_035c = 0;
    self->field_0361 = 0;
    return result;
}

HOOK(0x00748040, C_00868010_FreeBuffersAndReset, "auto");
