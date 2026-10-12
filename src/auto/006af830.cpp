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

// Game heap free (cdecl, pops nothing).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases one reference on the object at `self`; returns nonzero while more references remain.
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));

// Releases every entry of the pointer table and both owned buffers, then clears the references.
// Returns the last value of the primary buffer pointer (or the Mem_Free result if it was freed).
static int __fastcall DestroyAuxiliaryObject(int* self, void* /*edx*/) {
    uint32_t entryCount = CSF_AT(uint32_t, self, 0x08);

    for (uint32_t i = 0; i < entryCount; ++i) {
        if (reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x00))[i] != 0) {
            int stillReferenced;
            do {
                uintptr_t entryAddress = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x00))[i];
                stillReferenced = ReleaseObjectReference(
                    reinterpret_cast<void*>(entryAddress + 4), nullptr);
            } while (stillReferenced != 0);
            reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x00))[i] = 0;
        }
    }

    if (CSF_AT(uint32_t, self, 0x10) != 0) {
        Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10)));
        CSF_AT(uint32_t, self, 0x10) = 0;
    }

    int result = CSF_AT(int, self, 0x00);
    if (CSF_AT(uint32_t, self, 0x00) != 0) {
        result = Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x00)));
        CSF_AT(uint32_t, self, 0x00) = 0;
    }
    return result;
}

HOOK(0x006AF830, DestroyAuxiliaryObject, "auto");
