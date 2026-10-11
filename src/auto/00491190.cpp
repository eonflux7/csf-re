#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using std::uint32_t;

CSF_FUNC(char, __fastcall, SceneLoader_SetObjectTransform, 0x00490790, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxMatrix_SetFourRowsAndUpdate, 0x006ADA10, (S_006ada10* self, void* , uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));

static char __fastcall C_008342e8_SetObjectPlacement(
    C_008342e8* self, void* /*edx*/, uint32_t* position, float* direction)
{
    void* owner = reinterpret_cast<void*>(self->vGetField016c());
    void* transformSource =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[6])(owner, nullptr);

    uint32_t sourceMatrixState[2] = {};
    uint32_t* matrixView =
        reinterpret_cast<uint32_t* (__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, transformSource, 0)[35])(
                transformSource, nullptr, sourceMatrixState);

    float* sourceMatrix = CSF_AT(float*, matrixView, 0);
    float up[4];
    up[0] = CSF_AT(float, sourceMatrix, 0x20);
    up[1] = CSF_AT(float, sourceMatrix, 0x24);
    up[2] = CSF_AT(float, sourceMatrix, 0x28);

    if (CSF_AT(std::uint8_t, sourceMatrixState, 0x4))
        RwMatrixDestroy(
            reinterpret_cast<uint32_t*>(sourceMatrixState[0]));

    float right[3];
    right[0] = up[2] * direction[1] - up[1] * direction[2];
    right[1] = up[0] * direction[2] - up[2] * direction[0];
    right[2] = up[1] * direction[0] - up[0] * direction[1];
    NormalizeVector3(right, nullptr);

    const float upX = right[1] * direction[2] - right[2] * direction[1];
    const float upY = right[2] * direction[0] - right[0] * direction[2];
    const float upZ = right[0] * direction[1] - right[1] * direction[0];
    up[0] = upX;
    up[1] = upY;
    up[2] = upZ;
    up[3] = upX;

    uint32_t placementMatrixState[2] = {};
    placementMatrixState[0] =
        reinterpret_cast<std::uintptr_t>(RwMatrixCreate());
    CSF_AT(std::uint8_t, placementMatrixState, 0x4) = 1;

    // The packet also requests this address as a vtable binding.
    // There is no corresponding vtable store in the original.
    (void)CSF_VTBL(0x006ADA10);
    GfxMatrix_SetFourRowsAndUpdate(
        reinterpret_cast<S_006ada10*>(placementMatrixState), nullptr,
        reinterpret_cast<uint32_t*>(right),
        reinterpret_cast<uint32_t*>(direction),
        reinterpret_cast<uint32_t*>(up), position);
    SceneLoader_SetObjectTransform(self, nullptr, placementMatrixState);

    const char ownsMatrix =
        CSF_AT(char, placementMatrixState, 0x4);
    if (ownsMatrix)
        return static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<uint32_t*>(placementMatrixState[0])));
    return ownsMatrix;
}

HOOK(0x00491190, C_008342e8_SetObjectPlacement, "auto");
