#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __cdecl, RwMatrixRotateOneMinusCosineSine, 0x006246C0, (uint32_t* a1, float* a2, float a3, float a4, int a5));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_AxisZ, 0x008A7D84);

static int __fastcall GfxMatrix_RotateAroundAxisZ(
    int* self, void* /*edx*/, float angle)
{
    const float sine = std::sin(angle);
    uint32_t* matrix = CSF_AT(uint32_t*, self, 0);
    const float oneMinusCosine = g_TransitionOne - std::cos(angle);

    return reinterpret_cast<int>(
        RwMatrixRotateOneMinusCosineSine(
            matrix, &g_AxisZ, oneMinusCosine, sine, 2));
}

HOOK(0x006AE210, GfxMatrix_RotateAroundAxisZ, "auto");
