#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_GetBoundsCenter, 0x006AB660, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSector, 0x006D4720, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, GfxScene_ResolveResource, 0x006D59B0, (void* self, void* edx, char* String2));
CSF_FUNC(int, __cdecl, Rt2dObjectSetMTM, 0x007F8250, (uint32_t* a1, void* a2));
CSF_FUNC(uint32_t*, __cdecl, Rt2dObjectSetPosition, 0x007F83E0, (uint32_t* a1, uint32_t* a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static char __cdecl SceneMissionSetup_SetObjectBoundsTransform(
    uint32_t* object, S_006d4450* resourceName, float* inputBounds)
{
    alignas(8) uint8_t boundsBytes[0x18];
    alignas(8) uint8_t positionBytes[0x18]{};
    alignas(8) uint8_t matrixBytes[0x8];

    float* bounds = reinterpret_cast<float*>(boundsBytes);
    float* position = reinterpret_cast<float*>(positionBytes);

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
        void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
        void* accessor = reinterpret_cast<void*>(
            GlobalInt_ResolveAccessor(globalObject, nullptr));
        void* provider = GetCachedObjectProvider(accessor, nullptr);
        unsigned int resource = GfxScene_ResolveResource(
            provider, nullptr, reinterpret_cast<char*>(resourceName));
        void* sector = reinterpret_cast<void*>(
            SceneLoader_GetSector(provider, nullptr, static_cast<int>(resource)));

        if (CSF_AT(uint32_t, sector, 0x28))
        {
            SceneLoader_GetSector(provider, nullptr, static_cast<int>(resource));

            position[3] = 10.0f;
            position[4] = 10.0f;
            position[5] = 10.0f;
            position[0] = -10.0f;
            position[1] = -10.0f;
            position[2] = -10.0f;

            bounds[0] = 10.0f;
            bounds[1] = 10.0f;
            bounds[2] = 10.0f;
            bounds[3] = -10.0f;
            bounds[4] = -10.0f;
            bounds[5] = -10.0f;
        }
    }

    position[5] = bounds[2] - bounds[5];
    position[0] = (bounds[0] - bounds[3]) * g_HalfDurationScale;
    position[1] = (bounds[1] - bounds[4]) * g_HalfDurationScale;
    position[2] = position[5] * g_HalfDurationScale;
    Rt2dObjectSetPosition(object, reinterpret_cast<uint32_t*>(position));

    CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
    CSF_AT(char, matrixBytes, 0x4) = 1;

    float* center = SceneMissionSetup_GetBoundsCenter(boundsBytes, nullptr, position);
    GfxMatrix_CopyTranslationAndUpdate(
        matrixBytes, nullptr, reinterpret_cast<const uint32_t*>(center));

    Rt2dObjectSetMTM(object, CSF_AT(std::uint32_t*, matrixBytes, 0));

    char ownsMatrix = CSF_AT(char, matrixBytes, 0x4);
    if (ownsMatrix)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0)));
    return ownsMatrix;
}

HOOK(0x006B2000, SceneMissionSetup_SetObjectBoundsTransform, "auto");
