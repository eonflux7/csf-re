#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x691110: game heap deallocation (the exact allocator the original uses).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// 0x41E210: base-class teardown that runs after the vtable pointers are restored
// and before the optional free.
CSF_FUNC(int, __fastcall, sub_41E210, 0x0041E210, (void* self, void* edx));

// C_0082e6e8 destructor: restores its vtable (offset 0x00) and the secondary
// vtable (offset 0x04), runs the base teardown and, when the low bit of the flag
// argument is set, frees the object on the game heap. Returns the object's
// primary vtable slot address, as the original does (&this->__vftable).
static void** __fastcall C_0082e6e8_Destroy(C_0082e6e8* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x0082E6E8);
    self->field_0004 = CSF_ADDR_VT(0x0082E6CC);

    sub_41E210(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<void**>(&self->vtbl);
}

HOOK(0x0041FA20, C_0082e6e8_Destroy, "auto");
