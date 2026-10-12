#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, SceneLoader_RunDirectClumpFinish, 0x006DA9A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_RunDeferredClumpFinish, 0x006DB160, (void* self, void* edx));

static char __fastcall SceneLoader_FinishClumpLookup(S_006d24a0* self, void* /*edx*/)
{
    int baseValue = static_cast<int>(self->field_0034);
    uint32_t scaleMode = self->field_00c8;
    int resolvedValue = baseValue;

    if (scaleMode == 1)
    {
        resolvedValue = 2 * baseValue / 3;
    }
    else if (self->field_00c8 == 2)
    {
        resolvedValue = baseValue / 2;
    }

    self->field_010c = static_cast<uint32_t>(resolvedValue);

    if (baseValue <= 0 || scaleMode == 4)
    {
        return static_cast<char>(SceneLoader_RunDirectClumpFinish(self, nullptr));
    }
    return SceneLoader_RunDeferredClumpFinish(self, nullptr);
}

HOOK(0x006DB5D0, SceneLoader_FinishClumpLookup, "auto");
