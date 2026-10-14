#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x691110: game heap deallocation (the exact allocator used by the original destructor).
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* p));

// 0x53B5A0: destructor body (member teardown / base-destructor chain) that runs
// after CMenuGameOptionsPC restores its vtable pointers.
CSF_FUNC(int, __fastcall, CMenuGameOptions_Destruct_53B5A0, 0x53B5A0,
         (CMenuGameOptionsPC* self, void* edx));

// CMenuGameOptionsPC destructor: restores the primary vtable pointer (offset 0x00)
// and the secondary vtable pointer (offset 0x04), runs the destructor body and,
// when the low bit of the flag argument is set, frees the object on the game heap.
// Always returns the object pointer in eax.
static void* __fastcall CMenuGameOptionsPC_Destroy(CMenuGameOptionsPC* self, void* /*edx*/, char a2)
{
    CSF_AT(uint32_t, self, 0x0000) = CSF_ADDR_VT(0x008439B8);
    self->field_0004 = CSF_ADDR_VT(0x00843968);

    CMenuGameOptions_Destruct_53B5A0(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x544270, CMenuGameOptionsPC_Destroy, "auto");
