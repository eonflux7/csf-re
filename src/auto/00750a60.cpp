#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (RootDO).
CSF_FUNC(void, __fastcall, RootDO_BaseCleanup, 0x0067D030, (void* self, void* edx));

// Releases the transport storage held by a NetPostman (called only when the flag bit is set).
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Deleting destructor for NetPostman: runs the base cleanup, then releases the
// transport storage when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static NetPostman* __fastcall NetPostman_Destroy(NetPostman* self, void* /*edx*/, char flags)
{
    RootDO_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    return self;
}

HOOK(0x750A60, NetPostman_Destroy, "auto");
