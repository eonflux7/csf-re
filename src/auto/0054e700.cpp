#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Frees every non-null pointer in the owned array at +4 (count at +0x10),
// then frees the array itself with the debug allocator, and resets the list fields.
static void* __fastcall MpGameModes_FreeListAndReset(void* self, void* /*edx*/) {
    for (int i = 0; i < CSF_AT(int, self, 0x10); ++i) {
        uint32_t* slots = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x04));
        uint32_t element = slots[i];
        if (element != 0) {
            Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(element)));
        }
    }

    void* result = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x04)));
    CSF_AT(int, self, 0x10) = 0;
    if (result != nullptr) {
        result = reinterpret_cast<void*>(static_cast<uintptr_t>(Mem_FreeDebug(result)));
    }

    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    return result;
}

HOOK(0x0054E700, MpGameModes_FreeListAndReset, "auto");
