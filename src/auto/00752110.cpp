#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

static uint32_t* __fastcall C_00868ae0_ScalarDeletingDestructor(
    C_00868ae0* self, void* /*edx*/, char flags)
{
    CSF_AT(void**, self, 0) = CSF_VTBL(0x00868AE0);
    if ((flags & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00752110, C_00868ae0_ScalarDeletingDestructor, "auto");
