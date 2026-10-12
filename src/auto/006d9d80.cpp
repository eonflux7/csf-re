#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Grows a 16-byte-element array: allocates a new block sized for the current count plus
// extraSlots, copies the existing used elements over, bumps the count, and releases the
// previous block. Returns the new block when there was no previous block, otherwise the
// result of Mem_Free, matching the original.
static void* __fastcall Polygon_GrowEdgeArray(void* self, void* /*edx*/, int extraSlots)
{
    void* newBlock = Mem_Alloc(static_cast<std::size_t>(16 * (extraSlots + CSF_AT(int, self, 0x4))));
    int copiedCount = CSF_AT(int, self, 0x8);
    if (copiedCount) {
        std::memcpy(newBlock, CSF_AT(void*, self, 0x0), static_cast<std::size_t>(16 * copiedCount));
    }

    void* oldBlock = CSF_AT(void*, self, 0x0);
    CSF_AT(int, self, 0x4) += extraSlots;

    if (oldBlock == nullptr) {
        CSF_AT(void*, self, 0x0) = newBlock;
        return newBlock;
    }

    int freeResult = Mem_Free(oldBlock);
    CSF_AT(void*, self, 0x0) = nullptr;
    CSF_AT(void*, self, 0x0) = newBlock;
    return reinterpret_cast<void*>(static_cast<std::intptr_t>(freeResult));
}

HOOK(0x6D9D80, Polygon_GrowEdgeArray, "auto");
