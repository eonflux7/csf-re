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

CSF_FUNC(void, __fastcall, sub_798E40, 0x00798E40, (void* self, void* edx));
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

static uint32_t* __fastcall C_0085ff4c_Destroy(C_0085ff4c* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x0085FF4C);
    sub_798E40(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        NetSession_ReleaseTransportStorage((int)(uintptr_t)self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x6A8550, C_0085ff4c_Destroy, "auto");
