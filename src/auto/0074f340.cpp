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

CSF_FUNC(int, __cdecl, sub_64D6B0, 0x0064D6B0, (int a1, uint32_t a2));
CSF_GLOBAL(uint32_t, g_DdlObjectClassId, 0x008BC180);

static int __cdecl NetDdl_CreateAuxiliaryEvent(int a1)
{
    return sub_64D6B0(static_cast<int>(g_DdlObjectClassId), static_cast<uint32_t>(a1));
}

HOOK(0x74F340, NetDdl_CreateAuxiliaryEvent, "auto");
