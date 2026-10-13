#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_743640).
CSF_FUNC(int, __fastcall, sub_743640, 0x00743640, (void* self, void* edx));

// Releases the transport storage held by a net session object (cdecl, one stack argument).
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Deleting destructor for C_00867ab8: runs the base cleanup, then releases the
// transport storage when the low bit of the flags argument is set.
// Returns the object pointer (the vtable slot address, as the original does via &this->__vftable).
static void* __fastcall C_00867ab8_Destroy(C_00867ab8* self, void* /*edx*/, char flags)
{
    sub_743640(self, nullptr);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    return self;
}

HOOK(0x743690, C_00867ab8_Destroy, "auto");
