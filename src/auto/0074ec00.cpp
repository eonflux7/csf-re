#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base cleanup step run before the transport storage is released (sub_74EC30).
CSF_FUNC(int, __fastcall, C_00868658_CleanupBase, 0x0074EC30, (void* self, void* edx));

// Releases transport storage for this object (NetSession_ReleaseTransportStorage, cdecl, one stack arg).
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Deleting destructor for C_00868658: runs the base cleanup, then releases
// the transport storage when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_00868658_Destroy(C_00868658* self, void* /*edx*/, char flags)
{
    C_00868658_CleanupBase(self, nullptr);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    return self;
}

HOOK(0x74EC00, C_00868658_Destroy, "auto");
