#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee bound by address; name chosen here (signature kept from the packet).
CSF_FUNC(char, __fastcall, Particle_PrepareSlotEntry, 0x006E1AA0,
         (void* self, void* edx, void* a1, unsigned int a3, void* a2));

// __thiscall(S_006e20e0* this, uint32_t* out, uint32_t item, C_0082d410* ctx) ; ret 12
static uint32_t* __fastcall S_006e20e0_StoreSlotPointerForItem(S_006e20e0* self, void* /*edx*/,
                                                               uint32_t* a2, uint32_t a3,
                                                               C_0082d410* a4)
{
    // Index of `a3` inside the slot array (60-byte slots), or 0 when the array is empty.
    int32_t slotIndex = 0;
    uint32_t firstSlot = self->field_0004;
    if (firstSlot && (int32_t)(self->field_0008 - firstSlot) / 60)
        slotIndex = (int32_t)(a3 - firstSlot) / 60;

    Particle_PrepareSlotEntry(self, nullptr, reinterpret_cast<void*>(a3), 1u,
                              reinterpret_cast<void*>(a4));

    // The array base is reloaded after the call, as in the original.
    *a2 = self->field_0004 + 60u * static_cast<uint32_t>(slotIndex);
    return a2;
}

HOOK(0x006E20E0, S_006e20e0_StoreSlotPointerForItem, "auto");
