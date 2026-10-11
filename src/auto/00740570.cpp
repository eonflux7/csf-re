#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_7404E0).
CSF_FUNC(int, __fastcall, C_00867850_BaseCleanup, 0x007404E0, (void* self, void* edx));

// Net session transport release, called only when the object owns transport storage (flag bit 0).
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Deleting destructor for C_00867850: runs the base cleanup, then releases the
// transport storage when the low bit of the flags argument is set.
// Returns the object pointer (the vtable slot address, as the original does via &this->__vftable).
static void* __fastcall C_00867850_Destroy(C_00867850* self, void* /*edx*/, char flags)
{
    C_00867850_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    return self;
}

HOOK(0x740570, C_00867850_Destroy, "auto");
