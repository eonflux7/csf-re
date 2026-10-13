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

// 0x4F0E80: base-class teardown that runs after the vtable pointers are restored and
// before the optional free.
CSF_FUNC(int, __fastcall, C_0083f040_ReleaseBaseState, 0x004F0E80, (void* self, void* edx));

// C_0083f040 destructor: restores the primary vtable (offset 0x00) and the secondary
// vtable (offset 0x04), runs the base teardown and, when the low bit of the flag
// argument is set, frees the object on the game heap. Returns the object pointer.
static int* __fastcall C_0083f040_Destroy(C_0083f040* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x0083F040);
    self->field_0004 = CSF_ADDR_VT(0x0083F024);

    C_0083f040_ReleaseBaseState(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<int*>(self);
}

HOOK(0x004F1B90, C_0083f040_Destroy, "auto");

