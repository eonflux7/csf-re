#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

static uint32_t* __fastcall C_00868af0_Destroy(
    C_00868af0* self, void* /*edx*/, char flags)
{
    auto* ownedObject = CSF_AT(C_00868ae0*, self, 0x8);
    CSF_AT(void**, self, 0x0) = CSF_VTBL(0x00868AF0);

    if (ownedObject)
    {
        ownedObject->vSlot0(1);
        CSF_AT(uint32_t, self, 0x8) = 0;
    }

    CSF_AT(void*, self, 0xC) = CSF_VTBL(0x00868AD8);
    CSF_AT(void**, self, 0x0) = CSF_VTBL(0x00868AE0);

    if (flags & 1)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00752270, C_00868af0_Destroy, "auto");
