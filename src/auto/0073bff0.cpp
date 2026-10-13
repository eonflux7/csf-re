#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_73BF90).
CSF_FUNC(int, __fastcall, C_00867578_BaseCleanup, 0x0073BF90, (void* self, void* edx));

// Releases transport storage owned by the session (cdecl, one stack arg).
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Deleting destructor for C_008675c8: runs the base cleanup, then releases
// the transport storage when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_008675c8_Destroy(C_008675c8* self, void* /*edx*/, char flags)
{
    C_00867578_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(static_cast<int>(reinterpret_cast<uintptr_t>(self)));
    return self;
}

HOOK(0x73BFF0, C_008675c8_Destroy, "auto");
