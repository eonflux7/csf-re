#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(uint32_t, g_PropsSlotBase, 0x008BB478);

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static int __cdecl GfxObjectProps_AllocateSlotProps(int slotIndex)
{
    // Slot in the per-object table: base + index. Clear its owner pointer first.
    uint8_t* slot = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(g_PropsSlotBase + static_cast<uint32_t>(slotIndex)));
    CSF_AT(uint32_t, slot, 0) = 0;

    // 36-byte property record from the game heap.
    uint32_t* props = static_cast<uint32_t*>(Mem_Alloc(36));
    if (props != nullptr)
    {
        props[1] = 0;
        props[8] = 0;
        props[7] = 0xFFFFFFFFu;
        props[0] = 0xFFFFFFFFu;
        props[1] |= 0x40u;
        props[2] = 0;
        props[4] = 0;
        props[5] = 0;
        props[6] = 0xFFFFFFFFu;

        CSF_AT(uint32_t, slot, 4) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(props));
        return slotIndex;
    }

    CSF_AT(uint32_t, slot, 4) = 0;
    return slotIndex;
}

HOOK(0x006B83A0, GfxObjectProps_AllocateSlotProps, "auto");
