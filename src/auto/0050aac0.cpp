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

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AppMain_SetRenderQuality, 0x006DB620, (void* self, void* edx, int a2));

// Maps the stored render-quality option index (0..3) to the engine quality level.
// Any other index falls through to level 0.
static int OptionsFile_MapRenderQualityIndex(uint32_t optionIndex)
{
    switch (optionIndex)
    {
    case 0u:
        return 4;
    case 1u:
        return 2;
    case 2u:
        return 1;
    case 3u:
        return 0;
    default:
        return 0;
    }
}

static int __fastcall OptionsFile_ApplyRenderQuality(S_0050aac0* self, void* /*edx*/)
{
    int qualityLevel = OptionsFile_MapRenderQualityIndex(self->field_001c);

    int globalInt = GetGlobalInt();
    int managerId = ResolveClumpManager(globalInt, nullptr);
    int lookupValue = ResolveEntryLookupValue(
        reinterpret_cast<void*>(static_cast<uintptr_t>(managerId)), nullptr);
    char applyResult = AppMain_SetRenderQuality(
        reinterpret_cast<void*>(static_cast<uintptr_t>(lookupValue)), nullptr, qualityLevel);
    return static_cast<int>(applyResult);
}

HOOK(0x0050AAC0, OptionsFile_ApplyRenderQuality, "auto");
