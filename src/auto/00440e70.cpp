#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee bound by address; name chosen here (signature kept from the packet).
CSF_FUNC(unsigned int, __fastcall, Campaign_PrepareRecordEntry, 0x00440540,
         (void* self, void* edx, int a1, unsigned int a3, int arg8));

// __thiscall(S_00440e70* this, uint32_t* out, int item, int ctx) ; ret 12
static uint32_t* __fastcall S_00440e70_StoreRecordPointerForItem(S_00440e70* self, void* /*edx*/,
                                                                 uint32_t* a2, int a3, int a4)
{
    // Index of `a3` inside the record array (56-byte records), or 0 when the array is empty.
    int32_t recordIndex = 0;
    uint32_t firstRecord = self->field_0004;
    if (firstRecord && (int32_t)(self->field_0008 - firstRecord) / 56)
        recordIndex = (int32_t)((uint32_t)a3 - firstRecord) / 56;

    Campaign_PrepareRecordEntry(self, nullptr, a3, 1u, a4);

    // The array base is reloaded after the call, as in the original.
    *a2 = self->field_0004 + 56u * static_cast<uint32_t>(recordIndex);
    return a2;
}

HOOK(0x00440E70, S_00440e70_StoreRecordPointerForItem, "auto");
