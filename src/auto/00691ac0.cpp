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

// Releases each owned slot block (a pointer block whose first dword is a
// buffer pointer), then the slot table itself, and leaves the object with
// the base-class vtable installed.
static int __fastcall C_0085e5c4_ReleaseOwnedBlocks(C_0085e5c4* self, void* /*edx*/) {
    uint32_t slotIndex = 0;

    *(void***)self = CSF_VTBL(0x0085E5C4);

    if (self->field_000c != 0) {
        do {
            void** entry = reinterpret_cast<void**>(
                reinterpret_cast<uint32_t*>(self->field_0004)[slotIndex]);
            if (entry) {
                if (*entry) {
                    Mem_Free(*entry);
                    *entry = nullptr;
                }
                Mem_Free(entry);
                reinterpret_cast<uint32_t*>(self->field_0004)[slotIndex] = 0;
            }
            ++slotIndex;
        } while (slotIndex < self->field_000c);
    }

    int result = static_cast<int>(self->field_0004);
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }

    *(void***)self = CSF_VTBL(0x0083F4BC);
    return result;
}

HOOK(0x00691AC0, C_0085e5c4_ReleaseOwnedBlocks, "auto");
