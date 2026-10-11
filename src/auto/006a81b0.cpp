#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Releases the transport storage owned by the net session.
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Scalar deleting destructor for C_0085ff8c: restores this class' vtable and,
// when the low bit of the flags argument is set, releases the transport storage.
// Returns the object pointer (the original returns &this->__vftable in eax).
static uint32_t* __fastcall C_0085ff8c_ScalarDeletingDestructor(
    void* self, void* /*edx*/, char flags)
{
    CSF_AT(void**, self, 0) = CSF_VTBL(0x0085FF8C);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x6A81B0, C_0085ff8c_ScalarDeletingDestructor, "auto");
