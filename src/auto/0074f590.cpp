#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_74F5C0).
CSF_FUNC(int, __fastcall, C_00868768_BaseCleanup, 0x0074F5C0, (void* self, void* edx));

// Transport-level release used when the low bit of the flags argument is set.
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Deleting destructor for C_00868768: runs the base cleanup, then releases
// the transport storage when the low bit of the flags argument is set.
// Returns the object pointer, as the original does.
static void* __fastcall C_00868768_Destroy(C_00868768* self, void* /*edx*/, char flags)
{
    C_00868768_BaseCleanup(self, nullptr);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    return self;
}

HOOK(0x0074F590, C_00868768_Destroy, "auto");
