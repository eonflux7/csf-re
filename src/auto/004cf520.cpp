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

// 0x4D1140: base-class teardown (C_0083b958), runs after the vtable pointers are restored
// and before the optional free.
CSF_FUNC(int, __fastcall, C_0083b958_Destroy, 0x004D1140, (void* self, void* edx));

// C_0083b2e0 destructor: restores the primary vtable (offset 0x00) and the secondary
// vtable (offset 0x04), runs the base teardown and, when the low bit of the flag
// argument is set, frees the object on the game heap. Returns the object pointer.
static C_0083b2e0* __fastcall C_0083b2e0_Destroy(C_0083b2e0* self, void* /*edx*/, char a2)
{
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x0083B2E0);
    CSF_AT(uint32_t, self, 0x4) = CSF_ADDR_VT(0x0083B2C0);

    C_0083b958_Destroy(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x004CF520, C_0083b2e0_Destroy, "auto");
