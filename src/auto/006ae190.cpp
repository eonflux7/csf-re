#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __cdecl, RwMatrixRotateOneMinusCosineSine, 0x006246C0, (uint32_t* a1, float* a2, float a3, float a4, int a5));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_AxisX, 0x008A7D6C);

static double (__cdecl* s_ComputeSine)(double) =
    static_cast<double (__cdecl*)(double)>(&std::sin);
static double (__cdecl* s_ComputeCosine)(double) =
    static_cast<double (__cdecl*)(double)>(&std::cos);

static int __fastcall GfxMatrix_RotateAroundAxisX(
    int* self, void* /*edx*/, float angle)
{
    const float sine = static_cast<float>(s_ComputeSine(angle));
    uint32_t* matrix = CSF_AT(uint32_t*, self, 0);
    const float oneMinusCosine =
        static_cast<float>(g_TransitionOne - s_ComputeCosine(angle));

    return reinterpret_cast<int>(
        RwMatrixRotateOneMinusCosineSine(
            matrix, &g_AxisX, oneMinusCosine, sine, 2));
}

HOOK(0x006AE190, GfxMatrix_RotateAroundAxisX, "auto");
