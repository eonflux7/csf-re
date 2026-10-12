#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00860288_EnsureTransformHandle, 0x006B3A90, (void* self, void* edx));
CSF_FUNC(S_00402350*, __fastcall, GfxMatrix_TransformFrame, 0x006AC400, (uint32_t* self, void* , S_00402350* output, int* matrix));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Applies this object's frame transform to the output frame a2 and returns a2.
static void* __fastcall C_00860288_ApplyFrameTransform(C_00860288* self, void* /*edx*/, void* outputFrame)
{
    // Stack slot: dword result at [0..3] (ebp-8), ownership flag byte at [4] (ebp-4).
    alignas(8) uint8_t frameBytes[8];
    std::uint32_t* matrixSlot = reinterpret_cast<std::uint32_t*>(frameBytes);

    if (!self->field_0060)
        C_00860288_EnsureTransformHandle(self, nullptr);

    // Virtual call slot 0x8C on the embedded sub-object whose vptr is stored at +0x30.
    void** subVtbl = reinterpret_cast<void**>(self->field_0030);
    int* transformMatrix = reinterpret_cast<int*>(
        reinterpret_cast<void* (__fastcall*)(void*, void*, int*)>(subVtbl[0x8C / 4])(
            reinterpret_cast<void*>(&self->field_0030), nullptr, reinterpret_cast<int*>(matrixSlot)));

    GfxMatrix_TransformFrame(reinterpret_cast<std::uint32_t*>(self->field_0060), nullptr,
                             reinterpret_cast<S_00402350*>(outputFrame), transformMatrix);

    if (frameBytes[4])
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixSlot[0]));

    return outputFrame;
}

HOOK(0x006B41A0, C_00860288_ApplyFrameTransform, "auto");
