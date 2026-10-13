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

// Game heap allocator (MSVC __cdecl, one stack argument, pops 0).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Allocates the streaming buffer for this object and resets its cursor state.
// Returns 0 (int) like the original.
static int __fastcall C_008605d8_AllocateStreamBuffers(C_008605d8* self, void* /*edx*/, std::size_t bufferSize)
{
    self->field_00b4 = reinterpret_cast<uint32_t>(Mem_Alloc(bufferSize));
    self->field_00b8 = static_cast<uint32_t>(bufferSize);
    self->field_00bc = 0;
    self->field_00c0 = 0;
    return 0;
}

HOOK(0x006BC360, C_008605d8_AllocateStreamBuffers, "auto");
