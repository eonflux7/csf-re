#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (sub_73FC30).
CSF_FUNC(int, __fastcall, CleanupBaseState, 0x0073FC30, (void* self, void* edx));

// Releases the transport storage tied to this session object (NetSession_ReleaseTransportStorage).
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Deleting destructor for C_008677c0: runs the base cleanup, then releases the
// transport storage when the low bit of the flags argument is set.
// Returns the object pointer (the vtable slot address, as the original does via &this->__vftable).
static void* __fastcall C_008677c0_Destroy(C_008677c0* self, void* /*edx*/, char flags)
{
    CleanupBaseState(self, nullptr);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(static_cast<int>(reinterpret_cast<std::uintptr_t>(self)));
    return self;
}

HOOK(0x73FD00, C_008677c0_Destroy, "auto");
