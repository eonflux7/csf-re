#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee bound by address; name chosen here (signature kept from the packet).
CSF_FUNC(int, __fastcall, MissionScript_PrepareEntryAt, 0x004706A0,
         (void* self, void* edx, void* a1, unsigned int a3, void* arg8));

// __thiscall(S_00470ab0* this, uint32_t* out, void* item, void* ctx) ; ret 12
static uint32_t* __fastcall S_00470ab0_StoreElementPointerForItem(S_00470ab0* self, void* /*edx*/,
                                                                  uint32_t* a2, void* a3, void* a4)
{
    // Index of `a3` inside the element array (28-byte records), or 0 when the array is empty.
    int32_t entryIndex = 0;
    uint32_t firstEntry = self->field_0004;
    if (firstEntry && (int32_t)(self->field_0008 - firstEntry) / 28)
        entryIndex = (int32_t)((uint32_t)reinterpret_cast<uintptr_t>(a3) - firstEntry) / 28;

    MissionScript_PrepareEntryAt(self, nullptr, a3, 1u, a4);

    // The array base is reloaded after the call, as in the original.
    *a2 = self->field_0004 + 28u * static_cast<uint32_t>(entryIndex);
    return a2;
}

HOOK(0x00470AB0, S_00470ab0_StoreElementPointerForItem, "auto");
