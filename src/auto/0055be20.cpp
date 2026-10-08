#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// C_00845368::ConsumeFlag, __thiscall, tail-called. Pops nothing (ret 0).
CSF_FUNC(char, __fastcall, C_00845368_ConsumeFlag, 0x0055BD20, (void* self, void* edx));

// Mem_Free (game heap), __cdecl, one argument, caller cleans up.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the owned buffer at +0x008 when the flag at +0x004 is set, clears
// the two counters at +0x00C/+0x010, then tail-calls the base flag consumer.
static char __fastcall C_008453a4_ReleaseOwnedBufferAndConsumeFlag(C_008453a4* self, void* /*edx*/)
{
    if (self->field_0004)
    {
        uint32_t bufferAddress = self->field_0008;
        self->field_000c = 0;
        self->field_0010 = 0;
        if (bufferAddress)
        {
            Mem_Free(reinterpret_cast<void*>(bufferAddress));
            self->field_0008 = 0;
        }
        return C_00845368_ConsumeFlag(self, nullptr);
    }
    return static_cast<char>(self->field_0004);
}

HOOK(0x0055BE20, C_008453a4_ReleaseOwnedBufferAndConsumeFlag, "auto");
