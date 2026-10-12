#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee bound by address; name chosen here (signature kept from the packet).
CSF_FUNC(int, __fastcall, RecordArray_PrepareEntry, 0x0040BD50,
         (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));

// __thiscall(S_0040c070* this, uint32_t* out, char* item, uint32_t* ctx) ; ret 12
static uint32_t* __fastcall S_0040c070_StoreElementPointerForItem(S_0040c070* self, void* /*edx*/,
                                                                  uint32_t* a2, char* a3, uint32_t* a4)
{
    // Index of `a3` inside the element array (28-byte elements), or 0 when the array is empty.
    int32_t elementIndex = 0;
    uint32_t firstElement = self->field_0004;
    if (firstElement && (int32_t)(self->field_0008 - firstElement) / 28)
        elementIndex = (int32_t)((uint32_t)(uintptr_t)a3 - firstElement) / 28;

    RecordArray_PrepareEntry(self, nullptr, a3, 1u, a4);

    // The array base is reloaded after the call, as in the original.
    *a2 = self->field_0004 + 28u * static_cast<uint32_t>(elementIndex);
    return a2;
}

HOOK(0x0040C070, S_0040c070_StoreElementPointerForItem, "auto");
