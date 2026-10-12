#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee: stdcall, one entry argument (ret 4).
CSF_FUNC(int, __stdcall, DynamicObj_ReleaseEntry, 0x00442DF0, (int a1));
// Original game heap free (cdecl).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases every entry in the child list, clears the count, then frees the list buffer.
// Returns the buffer pointer value read before freeing, or Mem_Free's result when a buffer existed.
static int __fastcall C_00831074_ReleaseChildrenAndBuffer(C_00831074* self, void* /*edx*/) {
    uint32_t i = 0;
    for (i = 0; i < self->field_0014; ++i) {
        uint32_t* entries = reinterpret_cast<uint32_t*>(self->field_000c);
        int entry = static_cast<int>(entries[i]);
        DynamicObj_ReleaseEntry(entry);
    }

    int result = static_cast<int>(self->field_000c);
    self->field_0010 = 0;
    self->field_0014 = 0;
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(result)));
        self->field_000c = 0;
    }
    return result;
}

HOOK(0x00442F40, C_00831074_ReleaseChildrenAndBuffer, "auto");
