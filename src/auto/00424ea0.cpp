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

// 0x41E210: base-class (C_0082e598) vtable install and base init, run after the
// primary and secondary vtable pointers of this object are restored.
CSF_FUNC(int, __fastcall, C_0082e598_InstallVtablesAndInitBase, 0x0041E210, (C_0082e598* self, void* edx));

// C_0082fbe8 destructor: restores the primary vtable (offset 0x00) and the secondary
// vtable (offset 0x04), runs the base-class teardown and, when the low bit of the flag
// argument is set, frees the object on the game heap. Returns the object pointer.
static C_0082fbe8* __fastcall C_0082fbe8_Destroy(C_0082fbe8* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x0082FBE8);
    self->field_0004 = CSF_ADDR_VT(0x0082FBCC);

    C_0082e598_InstallVtablesAndInitBase(reinterpret_cast<C_0082e598*>(self), nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x00424EA0, C_0082fbe8_Destroy, "auto");
