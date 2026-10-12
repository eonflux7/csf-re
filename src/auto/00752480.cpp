#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

static uint32_t* __fastcall C_00868b04_Destroy(
    C_00868b04* self, void* /*edx*/, char flags)
{
    auto* ownedObject = reinterpret_cast<C_00868ae0*>(self->field_001c);
    CSF_AT(void**, self, 0x0) = CSF_VTBL(0x00868B04);

    if (ownedObject)
    {
        ownedObject->vSlot0(1);
        self->field_001c = 0;
    }

    CSF_AT(void*, self, 0x8) = CSF_VTBL(0x00868AD8);
    CSF_AT(void**, self, 0x0) = CSF_VTBL(0x00868AE0);

    if (flags & 1)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00752480, C_00868b04_Destroy, "auto");
