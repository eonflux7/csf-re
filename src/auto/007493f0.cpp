#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_749320).
CSF_FUNC(int, __fastcall, sub_749320, 0x00749320, (void* self, void* edx));

// NetZ transport storage release (NetSession_ReleaseTransportStorage).
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Deleting destructor for C_008680a0: runs the base cleanup, then releases
// the transport storage when the low bit of the flags argument is set.
// Returns the object pointer, as the original does by returning this.
static char* __fastcall C_008680a0_Destroy(C_008680a0* self, void* /*edx*/, char flags)
{
    sub_749320(self, nullptr);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    return reinterpret_cast<char*>(self);
}

HOOK(0x7493F0, C_008680a0_Destroy, "auto");
