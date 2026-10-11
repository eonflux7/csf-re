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

CSF_FUNC(int, __cdecl, S_006fdfe0_FindWorldFromClump, 0x0058DF40, (int a1));
CSF_FUNC(int, __cdecl, RpWorldRemoveClump, 0x0058E310, (int a1, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Removes every clump in the scene-loader list from its RpWorld before a render reset.
// Returns the list count as read after the last iteration.
static unsigned int __fastcall C_00860bb8_BeginRenderReset(C_00860bb8* self, void* /*edx*/)
{
    uint32_t result = self->field_001c;
    for (uint32_t i = 0; i < result; ++i)
    {
        C_0085e7b8* v4 = reinterpret_cast<C_0085e7b8**>(self->field_0014)[i];
        if (v4)
        {
            void* cachedObjectProvider = GetCachedObjectProvider(v4, nullptr);
            int clump = reinterpret_cast<int>(cachedObjectProvider);
            if (clump)
            {
                int worldFromClump = S_006fdfe0_FindWorldFromClump(clump);
                if (worldFromClump)
                    RpWorldRemoveClump(worldFromClump, clump);
            }
        }
        result = self->field_001c;
    }
    return result;
}

HOOK(0x6CBBC0, C_00860bb8_BeginRenderReset, "auto");
