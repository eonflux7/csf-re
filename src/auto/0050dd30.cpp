#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall Game_NextRandomValue(void* self, void* /*edx*/, int limit)
{
    const int32_t seed = CSF_AT(int32_t, self, 0xC4);
    const int32_t nextSeed =
        48271 * (seed % 44488) - 3399 * (seed / 44488);

    CSF_AT(int32_t, self, 0xC4) = nextSeed;
    if (nextSeed <= 0)
        CSF_AT(int32_t, self, 0xC4) = nextSeed + 0x7FFFFFFF;

    return CSF_AT(int32_t, self, 0xC4) % limit;
}

HOOK_KEEP_ECX(0x0050DD30, Game_NextRandomValue, 1, "auto");
