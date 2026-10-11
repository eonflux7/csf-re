#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Finishes the event descriptor embedded at +0x44 of the object.
CSF_FUNC(void, __cdecl, MpSpawnPoints_FinishEventDescriptor, 0x0040ECF0, ());
// Base-section cleanup run on the whole object.
CSF_FUNC(int, __fastcall, sub_64EBC0, 0x0064EBC0, (void* self, void* edx));
// Releases the transport storage of the session object.
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// Destructor body for C_00867770. Takes one stack argument (flags byte; bit 0 = free storage).
static uint32_t* __fastcall C_00867770_Destroy(C_00867770* self, void* /*edx*/, char flags)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00867770);

    MpSpawnPoints_FinishEventDescriptor();
    sub_64EBC0(self, nullptr);

    if ((flags & 1) != 0) {
        NetSession_ReleaseTransportStorage(static_cast<int>(reinterpret_cast<uintptr_t>(self)));
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0073F7A0, C_00867770_Destroy, "auto");
