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

// Per-object setup routine, __thiscall in the original (self in ecx, one stack arg).
CSF_FUNC(void, __fastcall, C_008631c8_PrepareAtomicForPipeline, 0x006D2310, (void* self, void* edx, int a2));

// RenderWare atomic pipeline setter (cdecl).
CSF_FUNC(int, __cdecl, RpAtomicSetPipeline, 0x0058CC90, (int a1, int a2));

static int __fastcall C_008631c8_SetAtomicPipelineIfNonNull(C_008631c8* self, void* /*edx*/, int a2)
{
    int result = 0;

    if (a2 != 0)
    {
        C_008631c8_PrepareAtomicForPipeline(self, nullptr, a2);
        result = RpAtomicSetPipeline(a2, static_cast<int>(reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x705FF0))));
    }
    return result;
}

HOOK(0x706040, C_008631c8_SetAtomicPipelineIfNonNull, "auto");
