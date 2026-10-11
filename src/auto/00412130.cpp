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

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

CSF_FUNC(int, __stdcall, sub_411E70, 0x00411E70, (int a1));

static int __fastcall C_0082dfd0_CopyPositionOrFallback(C_0082dfd0* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t* source;
    uint8_t fallbackBuffer[12];

    if (self->field_0088
        || (*reinterpret_cast<float*>(&self->field_0024) == g_ZeroVectorX
            && *reinterpret_cast<float*>(&self->field_0028) == g_ZeroVectorY
            && *reinterpret_cast<float*>(&self->field_002c) == g_ZeroVectorZ))
    {
        int fallbackResult = sub_411E70(static_cast<int>(reinterpret_cast<uintptr_t>(fallbackBuffer)));
        source = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(fallbackResult));
    }
    else
    {
        source = &self->field_0024;
    }

    a2[0] = source[0];
    a2[1] = source[1];
    int result = static_cast<int>(source[2]);
    a2[2] = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x00412130, C_0082dfd0_CopyPositionOrFallback, "auto");
