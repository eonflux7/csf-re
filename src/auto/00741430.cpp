#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Cleanup step run on the object before optional transport release (sub_741360).
CSF_FUNC(int, __fastcall, sub_741360, 0x00741360, (void* self, void* edx));

// Releases the transport storage held by the session (NetSession_ReleaseTransportStorage).
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Cleanup for C_008678a8: runs the base cleanup step, then releases transport
// storage when the low bit of the flags argument is set. Returns the object pointer.
static void* __fastcall C_008678a8_CleanupAndMaybeReleaseTransport(C_008678a8* self, void* /*edx*/, char flags)
{
    sub_741360(self, nullptr);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    return self;
}

HOOK(0x741430, C_008678a8_CleanupAndMaybeReleaseTransport, "auto");
