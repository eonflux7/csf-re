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

CSF_FUNC(void, __fastcall, C_00858730_ReleaseSubobjectState, 0x00662900, (void* self, void* edx));
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

static uint32_t* __fastcall C_00858730_Destroy(C_00858730* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x00858730);
    C_00858730_ReleaseSubobjectState(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        NetSession_ReleaseTransportStorage(reinterpret_cast<int>(self));
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x74ECB0, C_00858730_Destroy, "auto");
