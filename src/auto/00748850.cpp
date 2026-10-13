#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_748780).
CSF_FUNC(int, __fastcall, RunBaseCleanup, 0x00748780, (void* self, void* edx));

// Releases the transport storage attached to the object (NetSession_ReleaseTransportStorage).
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Deleting destructor for C_00867fc0: runs the base cleanup, then releases the
// transport storage when the low bit of the flags argument is set.
// Returns the object pointer.
static void* __fastcall C_00867fc0_Destroy(C_00867fc0* self, void* /*edx*/, char flags)
{
    RunBaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    return self;
}

HOOK(0x748850, C_00867fc0_Destroy, "auto");
