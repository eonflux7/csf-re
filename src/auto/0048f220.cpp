#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetEulerAngles, 0x006AE3A0, (float** self, void* , float* angles));

CSF_GLOBAL(float, g_RadiansToDegrees, 0x00833858);

static double __fastcall C_008342e8_ComputeResolvedObjectScalar(
    C_008342e8* self, void* /*edx*/)
{
    void* resolved = reinterpret_cast<void*>(self->vGetField016c());
    const bool usesOrientation =
        ((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, resolved, 0)[0xA8 / 4])(resolved, nullptr);

    resolved = reinterpret_cast<void*>(self->vGetField016c());
    void* object =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, resolved, 0)[0x18 / 4])(resolved, nullptr);

    if (!usesOrientation)
    {
        return ((double(__fastcall*)(void*, void*))
            CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);
    }

    float angles[3];
    alignas(4) std::uint8_t matrixStorage[0xC];

    float** matrix =
        ((float**(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, object, 0)[0x8C / 4])(
                object, nullptr, matrixStorage);

    float* eulerAngles = GfxMatrix_GetEulerAngles(matrix, nullptr, angles);
    const float scalar = eulerAngles[1] * g_RadiansToDegrees;
    const bool ownsMatrix = CSF_AT(std::uint8_t, matrixStorage, 0x4) != 0;
    CSF_AT(float, matrixStorage, 0x8) = scalar;

    if (ownsMatrix)
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixStorage, 0x0));

    return CSF_AT(float, matrixStorage, 0x8);
}

HOOK(0x0048F220, C_008342e8_ComputeResolvedObjectScalar, "auto");
