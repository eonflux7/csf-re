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

// 0x41E210: base-class teardown (TCharTask part) that runs after the vtable pointers
// are restored and before the optional free.
CSF_FUNC(int, __fastcall, C_0082e598_ReleaseCharTaskBase, 0x0041E210, (void* self, void* edx));

// C_0082e7c8 destructor: restores the primary vtable (offset 0x00) and the secondary
// vtable (offset 0x04), runs the base teardown and, when the low bit of the flag
// argument is set, frees the object on the game heap. Returns the object pointer.
static C_0082e7c8* __fastcall C_0082e7c8_Destroy(C_0082e7c8* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x0082E7C8);
    CSF_AT(uint32_t, self, 0x0004) = static_cast<uint32_t>(CSF_ADDR_VT(0x0082E7AC));

    C_0082e598_ReleaseCharTaskBase(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x00420230, C_0082e7c8_Destroy, "auto");
