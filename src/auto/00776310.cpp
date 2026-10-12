#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee bound by address; name chosen here (signature kept from the packet).
CSF_FUNC(char, __fastcall, S_006d4160_PrepareEntryInTable, 0x007759F0,
         (void* self, void* edx, char* a1, unsigned int a3, int arg8));

// __thiscall(S_00776310* this, uint32_t* out, char* item, int ctx) ; ret 12
static uint32_t* __fastcall S_00776310_StoreElementPointerForItem(S_00776310* self, void* /*edx*/,
                                                                  uint32_t* a2, char* a3, int32_t a4)
{
    // Index of `a3` inside the element array (60-byte elements), or 0 when the array is empty.
    int32_t elementIndex = 0;
    uint32_t firstElement = self->field_0004;
    if (firstElement && (int32_t)(self->field_0008 - firstElement) / 60)
        elementIndex = (int32_t)((uint32_t)(uintptr_t)a3 - firstElement) / 60;

    S_006d4160_PrepareEntryInTable(self, nullptr, a3, 1u, a4);

    // The array base is reloaded after the call, as in the original.
    *a2 = self->field_0004 + 60u * static_cast<uint32_t>(elementIndex);
    return a2;
}

HOOK(0x00776310, S_00776310_StoreElementPointerForItem, "auto");
