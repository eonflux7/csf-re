#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Clears the mode slot `slotIndex` of a TargetPoint. Each slot is a 44-byte record
// stored in an array whose base pointer lives at this+0x0C.
static int __fastcall TargetPoint_ClearMode(void* self, void* /*edx*/, int slotIndex)
{
    uint8_t* slotArray = CSF_AT(uint8_t*, self, 0x0C);
    uint8_t* slot = slotArray + 44 * slotIndex;

    if (!CSF_AT(uint8_t, slot, 0) || CSF_AT(uint32_t, slot, 4) != 0)
    {
        CSF_AT(uint32_t, slot, 24) = 0;
        CSF_AT(uint32_t, slot, 40) = 0;
    }
    CSF_AT(uint32_t, slot, 4) = 0;

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(slot));
}

HOOK(0x00406D40, TargetPoint_ClearMode, "auto");
