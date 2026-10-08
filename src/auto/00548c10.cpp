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

// 0x53EC00: destructor body (member teardown / base-destructor chain) that runs
// after C_00843ed0 restores its vtable pointers.
CSF_FUNC(void, __fastcall, CMenuMissionInfo_Destruct, 0x53EC00,
         (C_00843ed0* self, void* edx));

// CMenuExtras destructor: restores the primary vtable (offset 0x00)
// and the secondary vtable (offset 0x04), runs the destructor body and, when the
// low bit of the flag argument is set, frees the object on the game heap.
// Always returns the object pointer in eax.
static void* __fastcall CMenuExtras_Destroy(C_00843ed0* self, void* /*edx*/, char a2)
{
    CSF_AT(uint32_t, self, 0x0000) = CSF_ADDR_VT(0x00843ED0);
    self->field_0004 = 0x00843E80u;

    CMenuMissionInfo_Destruct(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x548C10, CMenuExtras_Destroy, "auto");
