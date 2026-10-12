#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_51AC90, 0x0051AC90, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the previously owned 16-byte block (after cleaning it up via sub_51AC90),
// then allocates a fresh zeroed 16-byte block and stores it in field_0004.
static int* __fastcall C_00871a18_ResetBlock16(C_00871a18* self, void* /*edx*/)
{
    uint32_t oldBlock = self->field_0004;
    if (oldBlock != 0) {
        sub_51AC90(reinterpret_cast<void*>(oldBlock), nullptr);
        Mem_Free(reinterpret_cast<void*>(oldBlock));
        self->field_0004 = 0;
    }

    int* block = static_cast<int*>(Mem_Alloc(16));
    if (block != nullptr) {
        block[1] = 0;
        block[2] = 0;
        block[3] = 0;
        self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(block));
        return block;
    }

    self->field_0004 = 0;
    return nullptr;
}

HOOK(0x0051ACD0, C_00871a18_ResetBlock16, "auto");
