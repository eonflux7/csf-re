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
CSF_GLOBAL(uint32_t, g_NetGhostClassId, 0x008BC174);

static int __cdecl MpSpawnPoints_CreateRecordImpl(int a1)
{
    return sub_64D6B0(static_cast<int>(g_NetGhostClassId), static_cast<uint32_t>(a1));
}

HOOK(0x74B3A0, MpSpawnPoints_CreateRecordImpl, "auto");
