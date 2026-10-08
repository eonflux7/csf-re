#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x691110: game heap deallocation (the exact allocator the original destructor uses).
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* p));

// 0x53AE10: destructor body (member teardown / base-destructor chain) that runs
// after CMenuOptionsPC restores its vtable pointers.
CSF_FUNC(void, __fastcall, CMenuOptions_Destruct_53AE10, 0x53AE10,
         (CMenuOptionsPC* self, void* edx));

// CMenuOptionsPC destructor: restores the primary vtable (offset 0x00) and the
// secondary vtable (offset 0x04), runs the destructor body and, when the low
// bit of the flag argument is set, frees the object on the game heap.
// Always returns the object pointer in eax.
static void* __fastcall CMenuOptionsPC_Destroy(CMenuOptionsPC* self, void* /*edx*/, char a2)
{
    CSF_AT(uint32_t, self, 0x0000) = CSF_ADDR_VT(0x00842858);
    self->field_0004 = 0x00842808u;

    CMenuOptions_Destruct_53AE10(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x53B8E0, CMenuOptionsPC_Destroy, "auto");
