#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(std::uint32_t*, __cdecl, GfxFrames_ComputeClumpLocalBoundingSphere, 0x006C0D70, (std::uint32_t* result, int clump));
CSF_FUNC(int, __fastcall, SceneLoader_GetSector, 0x006D4720, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, GfxScene_ResolveResource, 0x006D59B0, (void* self, void* edx, char* String2));
CSF_FUNC(int, __cdecl, Rt2dObjectSetMTM, 0x007F8250, (uint32_t* a1, void* a2));
CSF_FUNC(int, __cdecl, Rt2dObjectSetBoundingRadius, 0x007F9860, (int a1, int a2));

static char __cdecl GfxScene_ApplyResourceBoundingSphere(
    int object, S_006d4450* resourceName)
{
    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(global, nullptr));
    void* provider = GetCachedObjectProvider(accessor, nullptr);

    unsigned int resource = GfxScene_ResolveResource(
        provider, nullptr, reinterpret_cast<char*>(resourceName));

    void* sector = reinterpret_cast<void*>(
        SceneLoader_GetSector(provider, nullptr, static_cast<int>(resource)));
    if (!CSF_AT(std::uint32_t, sector, 0x28))
        return static_cast<char>(Rt2dObjectSetBoundingRadius(object, 0));

    sector = reinterpret_cast<void*>(
        SceneLoader_GetSector(provider, nullptr, static_cast<int>(resource)));

    alignas(8) std::uint8_t sphereBytes[0x10];
    GfxFrames_ComputeClumpLocalBoundingSphere(
        reinterpret_cast<std::uint32_t*>(sphereBytes),
        CSF_AT(int, CSF_AT(void*, sector, 0x20), 0));

    Rt2dObjectSetBoundingRadius(object, CSF_AT(int, sphereBytes, 0xC));

    alignas(8) std::uint8_t matrixBytes[0x8];
    CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, matrixBytes, 0x4) = 1;

    GfxMatrix_CopyTranslationAndUpdate(
        matrixBytes, nullptr,
        reinterpret_cast<const std::uint32_t*>(sphereBytes));

    Rt2dObjectSetMTM(
        reinterpret_cast<std::uint32_t*>(object),
        CSF_AT(std::uint32_t*, matrixBytes, 0));

    char ownsMatrix = static_cast<char>(
        CSF_AT(std::uint8_t, matrixBytes, 0x4));
    if (ownsMatrix)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0)));

    return ownsMatrix;
}

HOOK(0x006B2180, GfxScene_ApplyResourceBoundingSphere, "auto");
