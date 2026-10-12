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

CSF_FUNC(void, __fastcall, SceneLoader_PrepareLoadInterface, 0x00519C30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_51AC90, 0x0051AC90, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the current 16-byte state block held at +0x04 (if any), then allocates a
// fresh zeroed 16-byte block into +0x04. Returns the new block, or nullptr on allocation failure.
static uint32_t* __fastcall C_00840724_ReinitStateBlock(C_00840724* self, void* /*edx*/)
{
    SceneLoader_PrepareLoadInterface(self, nullptr);

    uint32_t oldBlock = self->field_0004;
    if (oldBlock) {
        sub_51AC90(reinterpret_cast<void*>(oldBlock), nullptr);
        Mem_Free(reinterpret_cast<void*>(oldBlock));
        self->field_0004 = 0;
    }

    uint32_t* block = static_cast<uint32_t*>(Mem_Alloc(16));
    if (block) {
        block[1] = 0;
        block[2] = 0;
        block[3] = 0;
        self->field_0004 = reinterpret_cast<uint32_t>(block);
        return block;
    }

    self->field_0004 = 0;
    return nullptr;
}

HOOK(0x0051AD20, C_00840724_ReinitStateBlock, "auto");
