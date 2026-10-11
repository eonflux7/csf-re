#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_GLOBAL(uint32_t, g_ObjectPropsBase, 0x008BB484);

static int __cdecl GfxObjectProps_ResetSlotAndAllocDword(int slotOffset) {
    uint8_t* slot = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(g_ObjectPropsBase)) + slotOffset;
    CSF_AT(uint32_t, slot, 0) = 0;

    uint32_t* allocated = static_cast<uint32_t*>(Mem_Alloc(4));
    if (allocated) {
        *allocated = 3;
        CSF_AT(uint32_t*, slot, 4) = allocated;
    } else {
        CSF_AT(uint32_t*, slot, 4) = nullptr;
    }
    return slotOffset;
}

HOOK(0x006B85C0, GfxObjectProps_ResetSlotAndAllocDword, "auto");
