#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the transport storage is released (sub_7416F0).
CSF_FUNC(int, __fastcall, NetCogibleDdl_CleanupBase, 0x007416F0, (void* self, void* edx));

// Releases the transport storage owned by this session object (NetSession_ReleaseTransportStorage).
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Deleting-style destructor for C_00867900: runs the base cleanup, then releases
// the transport storage when the low bit of the flags argument is set.
// Returns the object pointer, as the original does via `this` in eax.
static void* __fastcall C_00867900_CleanupAndMaybeRelease(C_00867900* self, void* /*edx*/, char flags)
{
    NetCogibleDdl_CleanupBase(self, nullptr);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    return self;
}

HOOK(0x741AF0, C_00867900_CleanupAndMaybeRelease, "auto");
