#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(void, __fastcall, C_00867890_ResetVtableAndClearField4, 0x00740840, (C_00867890* self, void* /*edx*/));
CSF_FUNC(int, __fastcall, C_00867b08_ClearBaseState, 0x00743640, (void* self, void* /*edx*/));
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// __thiscall(this, char flag), ret 4: written as a fastcall free function.
static uint32_t* __fastcall C_00867b08_ResetWithOptionalRelease(C_00867b08* self, void* /*edx*/, char releaseStorageFlag)
{
    // Install C_00867b08's vtable on the object itself.
    self->vtbl = CSF_VTBL(0x00867B08);

    // Embedded sub-object at +0xB8: install its vtable, then reset it.
    C_00867890* innerObject = reinterpret_cast<C_00867890*>(reinterpret_cast<uint8_t*>(self) + 0xB8);
    innerObject->vtbl = CSF_VTBL(0x00867AF8);
    C_00867890_ResetVtableAndClearField4(innerObject, nullptr);

    C_00867b08_ClearBaseState(self, nullptr);

    if ((releaseStorageFlag & 1) != 0)
    {
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00743780, C_00867b08_ResetWithOptionalRelease, "auto");
