#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x691110: game heap deallocation (the exact allocator the original destructor uses).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// 0x6B2830: base-class constructor/teardown of C_00860170, run after the vtable pointers are set.
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void* edx));

// C_00860af0 destructor: restores the primary vtable (offset 0x00) and the secondary
// data pointer (offset 0x04), runs the base teardown, and when the low bit of the flag
// argument is set, frees the object on the game heap. Returns the object pointer.
static C_00860af0* __fastcall C_00860af0_Destroy(C_00860af0* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x00860AF0);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x00860AEC);

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x006CA840, C_00860af0_Destroy, "auto");
