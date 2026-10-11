#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(uint32_t*, __cdecl, RwMatrixTransform, 0x00624C70, (uint32_t* a1, uint32_t* a2, int a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));
CSF_FUNC(int, __fastcall, GfxMatrix_AreTransformsDifferent, 0x006ADC80, (float** self, void* , float** other));
CSF_FUNC(char, __fastcall, WaterNode_PostUpdate, 0x006FF710, (void* self, void* edx));

static char __fastcall C_00863588_UpdateFrameTransform(C_00863588* self, void* /*edx*/)
{
    uint32_t* tmp = RwMatrixCreate();
    uint8_t* matOwner = reinterpret_cast<uint8_t*>(self->field_002c) + 8;
    char different = (char)GfxMatrix_AreTransformsDifferent(
        reinterpret_cast<float**>(matOwner), nullptr, reinterpret_cast<float**>(&tmp));
    RwMatrixDestroy(tmp);

    if (different) {
        uint8_t* frame = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self->field_0030, 12));
        if (CSF_AT(uint32_t, frame, 0x98)) {
            RwMatrixTransform(reinterpret_cast<uint32_t*>(frame + 0x10),
                              reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self->field_002c, 8)), 1);
            uint32_t* inv = RwMatrixCreate();
            Matrix_InvertWithStoredTarget(
                reinterpret_cast<S_006adb40*>(reinterpret_cast<uint8_t*>(self->field_002c) + 8),
                nullptr, reinterpret_cast<float**>(&inv));
            uint8_t* child = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, frame, 0x98));
            RwMatrixTransform(reinterpret_cast<uint32_t*>(child + 0x10), inv, 2);
            RwMatrixDestroy(inv);
        }
        RwFrameUpdateObjects(static_cast<int>(reinterpret_cast<uintptr_t>(frame)));
    }
    return WaterNode_PostUpdate(self, nullptr);
}

HOOK(0x0070A5D0, C_00863588_UpdateFrameTransform, "auto");
