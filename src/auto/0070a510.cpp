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

CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(uint32_t*, __cdecl, RwMatrixTransform, 0x00624C70, (uint32_t* a1, uint32_t* a2, int a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));
CSF_FUNC(int, __fastcall, GfxMatrix_AreTransformsDifferent, 0x006ADC80, (float** self, void* , float** other));
CSF_FUNC(bool, __fastcall, sub_6FF680, 0x006FF680, (void* self, void* edx));

static bool __fastcall WaterC_00863588_SyncFrameTransform(C_00863588* self, void* /*edx*/)
{
    // 8-byte stack object: matrix pointer at +0, ownership flag at +4
    alignas(8) uint8_t matrixHolder[8];
    float** matrixSlot = reinterpret_cast<float**>(matrixHolder);

    CSF_AT(uint32_t*, matrixHolder, 0) = RwMatrixCreate();
    CSF_AT(uint8_t, matrixHolder, 4) = 1;
    uint8_t differs = (uint8_t)GfxMatrix_AreTransformsDifferent(
        reinterpret_cast<float**>(reinterpret_cast<uint8_t*>(self->field_002c) + 8), nullptr, matrixSlot);
    if (CSF_AT(uint8_t, matrixHolder, 4))
        RwMatrixDestroy(CSF_AT(uint32_t*, matrixHolder, 0));

    if (differs)
    {
        uint32_t frame = CSF_AT(uint32_t, self->field_0030, 12);
        uint32_t target = CSF_AT(uint32_t, frame, 0x98);
        if (target)
        {
            RwMatrixTransform(reinterpret_cast<uint32_t*>(target + 0x10),
                              CSF_AT(uint32_t*, self->field_002c, 8), 2);
            CSF_AT(uint32_t*, matrixHolder, 0) = RwMatrixCreate();
            CSF_AT(uint8_t, matrixHolder, 4) = 1;
            Matrix_InvertWithStoredTarget(
                reinterpret_cast<S_006adb40*>(reinterpret_cast<uint8_t*>(self->field_002c) + 8),
                nullptr, matrixSlot);
            RwMatrixTransform(reinterpret_cast<uint32_t*>(frame + 0x10),
                              CSF_AT(uint32_t*, matrixHolder, 0), 1);
            if (CSF_AT(uint8_t, matrixHolder, 4))
                RwMatrixDestroy(CSF_AT(uint32_t*, matrixHolder, 0));
        }
        RwFrameUpdateObjects((int)frame);
    }
    self->field_0038 = 1;
    return sub_6FF680(self, nullptr);
}

HOOK(0x0070A510, WaterC_00863588_SyncFrameTransform, "auto");
