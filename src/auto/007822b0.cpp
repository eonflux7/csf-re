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

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Frees every node payload (node[2]) through CoreMemory_Free, then unlinks and frees
// every node of the circular list rooted at field_0008 and resets the list to empty.
// Sets the count field to 0 and field_0010 to the bit pattern of 0.5f.
static uint32_t* __fastcall S_007822b0_ClearAndFreeList(S_007822b0* self, void* /*edx*/)
{
    uint32_t* head = reinterpret_cast<uint32_t*>(self->field_0008);
    uint32_t* node = reinterpret_cast<uint32_t*>(head[0]);
    if (node != head) {
        do {
            if (node[2]) {
                CoreMemory_Free(reinterpret_cast<void*>(node[2]));
            }
            node = reinterpret_cast<uint32_t*>(node[0]);
        } while (node != reinterpret_cast<uint32_t*>(self->field_0008));
    }

    head = reinterpret_cast<uint32_t*>(self->field_0008);
    uint32_t* result = reinterpret_cast<uint32_t*>(head[0]);
    head[0] = reinterpret_cast<uint32_t>(head);
    head[1] = reinterpret_cast<uint32_t>(head);

    self->field_000c = 0;
    if (result != reinterpret_cast<uint32_t*>(self->field_0008)) {
        uint32_t* next;
        do {
            next = reinterpret_cast<uint32_t*>(result[0]);
            Mem_FreeDebug(reinterpret_cast<void*>(result));
            result = next;
        } while (next != reinterpret_cast<uint32_t*>(self->field_0008));
    }

    self->field_0010 = 0x3f000000u;
    return result;
}

HOOK(0x007822B0, S_007822b0_ClearAndFreeList, "auto");
