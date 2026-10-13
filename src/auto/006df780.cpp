#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x691110: game heap deallocation (the exact allocator the original uses).
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// 0x6B2830: base-class constructor/teardown of C_00860170, run after the vtable
// pointers are restored and before the optional free.
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void* edx));

// C_00861928 scalar-deleting destructor: restores the primary vtable (offset 0x00)
// and the secondary vtable (offset 0x04), runs the base teardown and, when the low
// bit of the flag argument is set, frees the object on the game heap. Returns the
// object pointer.
static C_00861928* __fastcall C_00861928_Destroy(C_00861928* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x00861928);
    CSF_AT(uint32_t, self, 0x004) = CSF_ADDR_VT(0x00861924);

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((a2 & 1) != 0)
        CoreMemory_Free(self);

    return self;
}

HOOK(0x006DF780, C_00861928_Destroy, "auto");
