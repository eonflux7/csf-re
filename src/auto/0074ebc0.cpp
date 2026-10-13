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

CSF_FUNC(uint32_t*, __fastcall, ObjectBase_Initialize, 0x0064EBA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Registry_Attach, 0x0065E310, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008310b8_ClearState, 0x00744380, (C_008310b8* self, void*));
CSF_FUNC(int, __fastcall, C_008310b8_ResetCallbackState, 0x00744510, (C_008310b8* self, void*));
CSF_FUNC(int, __fastcall, C_008310b8_ClearCallbackState, 0x007445B0, (C_008310b8* self, void*));

CSF_GLOBAL(uint32_t, g_NetControlClassId, 0x008BC17C);

static int __fastcall C_00868658_Construct(C_00868658* self, void* /*edx*/)
{
    ObjectBase_Initialize(reinterpret_cast<void*>(self), nullptr);
    *(void***)self = CSF_VTBL(0x00868658);
    C_008310b8_ClearCallbackState(
        reinterpret_cast<C_008310b8*>(reinterpret_cast<uint8_t*>(self) + 0x44), nullptr);
    C_008310b8_ClearState(
        reinterpret_cast<C_008310b8*>(reinterpret_cast<uint8_t*>(self) + 0x58), nullptr);
    C_008310b8_ResetCallbackState(
        reinterpret_cast<C_008310b8*>(reinterpret_cast<uint8_t*>(self) + 0xA8), nullptr);
    Registry_Attach(reinterpret_cast<uint8_t*>(self) + 0x2C, nullptr, static_cast<int>(g_NetControlClassId));
    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x0074EBC0, C_00868658_Construct, "auto");
