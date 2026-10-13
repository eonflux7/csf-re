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

CSF_FUNC(int, __fastcall, BaseDestructor_7A2DE0, 0x007A2DE0, (void* self, void* edx));
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

static uint32_t* __fastcall C_00868050_Destroy(C_00868050* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x00868050);
    BaseDestructor_7A2DE0(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        NetSession_ReleaseTransportStorage(static_cast<int>(reinterpret_cast<uintptr_t>(self)));
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x748F70, C_00868050_Destroy, "auto");
