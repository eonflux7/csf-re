#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x691110: game heap deallocation (the exact allocator/free the original destructor uses).
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* p));

// 0x53CCE0: CMenuSoundOptions teardown body (member clean-up / base-destructor chain)
// that runs after CMenuSoundOptionsPC restores its vtable pointers.
CSF_FUNC(int, __fastcall, CMenuSoundOptions_Destruct_53CCE0, 0x53CCE0,
         (CMenuSoundOptionsPC* self, void* edx));

// CMenuSoundOptionsPC destructor: restores the primary vtable at +0x00 and the
// secondary (CMenuSoundOptions sub-object) vtable at +0x04, runs the
// CMenuSoundOptions clean-up body and, when the low bit of the flag argument is
// set, frees the object on the game heap (scalar deleting destructor).
// Always returns the object pointer in eax.
static CMenuSoundOptionsPC* __fastcall CMenuSoundOptionsPC_Destroy(
    CMenuSoundOptionsPC* self, void* /*edx*/, char a2)
{
    CSF_AT(uint32_t, self, 0x0000) = CSF_ADDR_VT(0x00843DB8);
    self->field_0004 = 0x00843D68u;

    CMenuSoundOptions_Destruct_53CCE0(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x548BA0, CMenuSoundOptionsPC_Destroy, "auto");
