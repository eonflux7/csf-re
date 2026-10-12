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

// 0x6D2520: base-class teardown that runs after the vtable pointers are restored and
// before the optional free.
CSF_FUNC(void, __fastcall, C_008631c8_ReleaseBaseState, 0x006D2520, (void* self, void* edx));

// C_008631c8 destructor: restores the primary vtable (offset 0x00) and the secondary
// vtable (offset 0x04), runs the base teardown and, when the low bit of the flag
// argument is set, frees the object on the game heap. Returns the object pointer.
static C_008631c8* __fastcall C_008631c8_Destroy(C_008631c8* self, void* /*edx*/, char a2)
{
    *(void***)self = CSF_VTBL(0x008631C8);
    CSF_AT(uint32_t, self, 0x04) = CSF_ADDR_VT(0x008631C4);

    C_008631c8_ReleaseBaseState(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x00706010, C_008631c8_Destroy, "auto");
