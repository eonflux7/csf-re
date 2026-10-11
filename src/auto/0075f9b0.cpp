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

// Game heap free (debug variant) and release variant, bound by address.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Clears/rebuilds the node index held at this+0x14. Unnamed original; signature kept as given.
CSF_FUNC(int*, __fastcall, WaypointGraph_ClearNodeIndex, 0x0075F4A0,
         (uint32_t* a1, int a2, int* a3, int a4, int a5));

static int __fastcall WaypointGraph_ReleaseNodeTable(void* self, void* /*edx*/)
{
    uint8_t* base = static_cast<uint8_t*>(self);

    // Pass 1: index sub-object at +0x14, with the node pointer stored at +0x18.
    int* nodePtr = reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x18));
    int* indexHead = nullptr;
    // a3 is the address of the local out-slot (original: lea eax, [ebp-4]; push eax).
    WaypointGraph_ClearNodeIndex(reinterpret_cast<uint32_t*>(base + 0x14), 0,
                                 reinterpret_cast<int*>(&indexHead),
                                 *nodePtr,
                                 static_cast<int>(reinterpret_cast<intptr_t>(nodePtr)));

    // Free the node table at +0x18 (re-read after the call).
    if (CSF_AT(uint32_t, self, 0x18)) {
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x18)));
    }
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;

    // Free the bucket block at +0x04 if present.
    uint32_t bucket = CSF_AT(uint32_t, self, 0x04);
    int result = static_cast<int>(bucket);
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(bucket));
        CSF_AT(uint32_t, self, 0x04) = 0;
    }
    return result;
}

HOOK(0x75F9B0, WaypointGraph_ReleaseNodeTable, "auto");
