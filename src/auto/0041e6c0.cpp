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

// 0x4DB3C0: base-class (TCharTask / C_0082d410) teardown that runs after the vtable
// pointers are restored and before the optional free.
CSF_FUNC(void, __fastcall, C_0082d410_ReleaseBaseMembers, 0x004DB3C0, (void* self, void* edx));

// C_0082e598 destructor: restores the primary vtable (offset 0x00) and the secondary
// vtable (offset 0x04), runs the base teardown and, when the low bit of the flag
// argument is set, frees the object on the game heap. Returns the object pointer.
static C_0082e598* __fastcall C_0082e598_Destroy(C_0082e598* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x0082E598);
    self->field_0004 = CSF_ADDR_VT(0x0082E57C);

    C_0082d410_ReleaseBaseMembers(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x0041E6C0, C_0082e598_Destroy, "auto");
