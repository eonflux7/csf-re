#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, C_0082e520_Destroy, 0x0041E110, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Scalar deleting destructor for C_0082df28 (derived from C_0082d410).
// C_0082df28 is not declared in auto_classes.h and has no known fields, so the
// object is handled as an opaque block whose first dword is the vtable pointer.
// Sets vtable C_0082df28, runs the C_0082e520 base teardown, sets vtable
// C_0082d410, frees the block when bit 0 of a2 is set, and returns the object.
static uint32_t* __fastcall C_0082df28_ScalarDestructor(void* self, void* /*edx*/, char a2)
{
    *(void***)self = CSF_VTBL(0x0082DF28);
    C_0082e520_Destroy(self, nullptr);
    *(void***)self = CSF_VTBL(0x0082D410);
    if ((a2 & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x411640, C_0082df28_ScalarDestructor, "auto");
