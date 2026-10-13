#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Releases the transport storage owned by the NetSession. Takes the object pointer as its argument.
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Scalar deleting destructor for C_00858930: restores this class' vtable and,
// when the low bit of the flags argument is set, releases transport storage.
// Returns the object pointer (the original returns this in eax).
static uint32_t* __fastcall C_00858930_ScalarDeletingDestructor(
    void* self, void* /*edx*/, char flags)
{
    CSF_AT(void**, self, 0) = CSF_VTBL(0x00858930);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(static_cast<int>(reinterpret_cast<uintptr_t>(self)));
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x74FD40, C_00858930_ScalarDeletingDestructor, "auto");
