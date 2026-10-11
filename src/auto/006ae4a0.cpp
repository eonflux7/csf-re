#include <cmath>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(float*, __fastcall, S006adb40_ComputeMatrix, 0x006ADB40, (void* self, void* edx, void** a2));

// Vtable of S_006adb40, referenced by the original's call to 0x6ADB40 (constructor chain).
static const void* const s_S006adb40Vtable = CSF_VTBL(0x006ADB40);

static float __fastcall ComputeTransformedRadialDistance(S_006adb40* self, void* /*edx*/, int pointAddr)
{
    std::uint32_t* matrix = RwMatrixCreate();
    bool ownsMatrix = true;

    S006adb40_ComputeMatrix(self, nullptr, reinterpret_cast<void**>(&matrix));

    float transformed[3] = {0.0f, 0.0f, 0.0f};
    RwV3dTransformPoints(reinterpret_cast<int>(&transformed[0]), pointAddr, 1,
                         reinterpret_cast<int>(matrix));

    float radial = std::sqrt(transformed[0] * transformed[0] + transformed[1] * transformed[1]);
    float pointZ = *reinterpret_cast<float*>(static_cast<std::uintptr_t>(pointAddr) + 12);
    float result = radial - self->field_0008 - pointZ;

    if (ownsMatrix)
        RwMatrixDestroy(matrix);

    return result;
}

HOOK(0x006AE4A0, ComputeTransformedRadialDistance, "auto");
