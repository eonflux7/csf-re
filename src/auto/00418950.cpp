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

// Grows a 32-byte-element array owned by the vehicle wheel list: allocates a new block
// sized for the current count plus extraSlots, copies the old elements over, bumps the
// count, and releases the previous block. Returns the new block (or Mem_Free's result
// when a previous block existed, matching the original).
static void* __fastcall Vehicle_ReserveWheelSlots(void* self, void* /*edx*/, int extraSlots)
{
    void* newBlock = Mem_Alloc(static_cast<std::size_t>(32 * (extraSlots + CSF_AT(int, self, 0x4))));
    int copiedCount = CSF_AT(int, self, 0x8);
    if (copiedCount) {
        std::memcpy(newBlock, CSF_AT(void*, self, 0x0), static_cast<std::size_t>(32 * copiedCount));
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

HOOK(0x418950, Vehicle_ReserveWheelSlots, "auto");
