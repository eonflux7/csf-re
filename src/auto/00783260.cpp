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

// Original: 0x00782A70 sub_782A70, __cdecl, 4 args.
CSF_FUNC(int, __cdecl, ObjectPayload_Release, 0x00782A70, (int* a1, int* a2, int a3, int a4));
// Original: 0x00690E60 Mem_FreeDebug, __cdecl.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
// Original: 0x00691110 CoreMemory_Free, __cdecl.
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// __thiscall with one stack argument (ret 4). self = ecx, freeSelfFlags = [ebp+8].
// Releases the payload at self+4 (through ObjectPayload_Release, then Mem_FreeDebug if
// still set), clears the three owned-pointer slots, and frees self when bit 0 is set.
static void* __fastcall ReleaseOwnedBlock(void* self, void* /*edx*/, int freeSelfFlags)
{
    int* owned = CSF_AT(int*, self, 0x4);
    if (owned) {
        ObjectPayload_Release(owned, CSF_AT(int*, self, 0x8), freeSelfFlags, reinterpret_cast<int>(self));
        if (CSF_AT(int*, self, 0x4)) {
            Mem_FreeDebug(CSF_AT(void*, self, 0x4));
        }
    }

    CSF_AT(int*, self, 0x4) = nullptr;
    CSF_AT(int*, self, 0x8) = nullptr;
    CSF_AT(int, self, 0xC) = 0;

    if ((freeSelfFlags & 1) != 0) {
        CoreMemory_Free(self);
    }
    return self;
}

HOOK(0x00783260, ReleaseOwnedBlock, "auto");
