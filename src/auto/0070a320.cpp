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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, Water_ReleaseResources, 0x0070CC90, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Water_ClearSubobjects, 0x0070A1C0, (void* self, void* edx));

// Scalar deleting destructor: installs the data table pointer, releases the
// resources, clears the sub-objects and frees the block when the low bit of
// the flag argument is set.
static int* __fastcall Water_DestroyAndFreeIfFlag(S_006fdfe0* self, void* /*edx*/, char a2)
{
    self->field_0000 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x00863548)));

    Water_ReleaseResources(self, nullptr);
    Water_ClearSubobjects(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<int*>(self);
}

HOOK(0x0070A320, Water_DestroyAndFreeIfFlag, "auto");
