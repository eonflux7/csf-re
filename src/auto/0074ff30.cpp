#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Releases the transport storage owned by a NetSession object (cdecl, one int argument).
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Restores C_00858948's vtable on the object. When the low bit of the flags
// argument is set, first releases the transport storage. Returns the object
// pointer (the original returns this in eax).
static uint32_t* __fastcall C_00858948_DestroyAndReleaseStorage(
    void* self, void* /*edx*/, char flags)
{
    CSF_AT(void**, self, 0) = CSF_VTBL(0x00858948);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x74FF30, C_00858948_DestroyAndReleaseStorage, "auto");
