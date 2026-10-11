#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Cleanup routine called with the object pointer when the low flag bit is set.
CSF_FUNC(void, __cdecl, sub_6471C0, 0x006471C0, (int a2));

// Scalar deleting destructor for C_008676a4: restores this class' vtable and,
// when the low bit of the flags argument is set, runs the cleanup routine on
// the object. Returns the object pointer (the original returns this in eax).
static uint32_t* __fastcall C_008676a4_ScalarDeletingDestructor(
    void* self, void* /*edx*/, char flags)
{
    CSF_AT(void**, self, 0) = CSF_VTBL(0x008676A4);
    if ((flags & 1) != 0)
        sub_6471C0(reinterpret_cast<int>(self));
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x73CF40, C_008676a4_ScalarDeletingDestructor, "auto");
