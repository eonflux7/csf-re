#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_GetBoundsCenter, 0x006AB660, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __fastcall, GfxMatrix_SetRotationAroundX, 0x006AE250, (S_006ada10* self, void* , float angle));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSector, 0x006D4720, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, GfxScene_ResolveResource, 0x006D59B0, (void* self, void* edx, char* String2));
CSF_FUNC(int, __cdecl, Rt2dObjectSetMTM, 0x007F8250, (uint32_t* a1, void* a2));
CSF_FUNC(uint32_t*, __cdecl, GfxScene_Set2dObjectSize, 0x007F9B40, (uint32_t* a1, int a2, int a3));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static char __cdecl GfxScene_Fit2dObjectToBounds(
    uint32_t* object, S_006d4450* resourceName, float* inputBounds)
{
    alignas(8) uint8_t boundsBytes[0x18];
    float* bounds = reinterpret_cast<float*>(boundsBytes);
    alignas(8) uint8_t centerBytes[0x18];
    float* center = reinterpret_cast<float*>(centerBytes);
    alignas(8) uint8_t matrixBytes[0x8];
    S_006ada10& matrix = *reinterpret_cast<S_006ada10*>(matrixBytes);

    bounds[0] = inputBounds[0];
    bounds[1] = inputBounds[1];
    bounds[3] = inputBounds[3];
    bounds[2] = inputBounds[2];
    bounds[4] = inputBounds[4];
    bounds[5] = inputBounds[5];

    if (bounds[3] == bounds[0] &&
        bounds[4] == bounds[1] &&
        bounds[5] == bounds[2])
    {
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* accessor = reinterpret_cast<void*>(
            GlobalInt_ResolveAccessor(global, nullptr));
        void* provider = GetCachedObjectProvider(accessor, nullptr);
        unsigned int resource = GfxScene_ResolveResource(
            provider, nullptr, reinterpret_cast<char*>(resourceName));

        void* sector = reinterpret_cast<void*>(
            SceneLoader_GetSector(provider, nullptr, static_cast<int>(resource)));
        if (CSF_AT(uint32_t, sector, 0x28))
        {
            SceneLoader_GetSector(provider, nullptr, static_cast<int>(resource));
            center[3] = 10.0f;
            center[4] = 10.0f;
            center[5] = 10.0f;
            center[0] = -10.0f;
            center[1] = -10.0f;
            center[2] = -10.0f;
            bounds[0] = 10.0f;
            bounds[1] = 10.0f;
            bounds[2] = 10.0f;
            bounds[3] = -10.0f;
            bounds[4] = -10.0f;
            bounds[5] = -10.0f;
        }
    }

    center[3] = bounds[0] - bounds[3];
    center[4] = bounds[1] - bounds[4];
    center[5] = bounds[2] - bounds[5];

    float horizontalExtent = center[3] < center[5] ? center[5] : center[3];
    float halfExtent = horizontalExtent * g_HalfDurationScale;
    float verticalOffset = center[4] * g_HalfDurationScale - halfExtent;
    int halfExtentBits;
    int verticalOffsetBits;
    std::memcpy(&halfExtentBits, &halfExtent, sizeof(halfExtentBits));
    std::memcpy(&verticalOffsetBits, &verticalOffset, sizeof(verticalOffsetBits));
    GfxScene_Set2dObjectSize(object, halfExtentBits, verticalOffsetBits);

    matrix.field_0000 =
        reinterpret_cast<decltype(matrix.field_0000)>(RwMatrixCreate());
    CSF_AT(uint8_t, &matrix, 0x4) = 1;
    GfxMatrix_SetRotationAroundX(&matrix, nullptr, 1.5707964f);

    float* translation = SceneMissionSetup_GetBoundsCenter(bounds, nullptr, center);
    GfxMatrix_CopyTranslationAndUpdate(
        &matrix, nullptr, reinterpret_cast<const uint32_t*>(translation));
    Rt2dObjectSetMTM(object, reinterpret_cast<void*>(matrix.field_0000));

    char ownsMatrix = static_cast<char>(CSF_AT(uint8_t, &matrix, 0x4));
    if (ownsMatrix)
        return static_cast<char>(
            RwMatrixDestroy(reinterpret_cast<uint32_t*>(matrix.field_0000)));
    return ownsMatrix;
}

HOOK(0x006B2240, GfxScene_Fit2dObjectToBounds, "auto");
