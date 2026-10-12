#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));
CSF_FUNC(int, __cdecl, RpClumpRender, 0x0058EEB0, (int a1));
CSF_FUNC(int, __cdecl, RwFrameTransform, 0x00623860, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisX, 0x006AE190, (int* self, void* , float angle));
CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisY, 0x006AE1D0, (int* self, void* , float angle));
CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisZ, 0x006AE210, (int* self, void* , float angle));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetEulerAngles, 0x006AE3A0, (float** self, void* , float* angles));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

static int __cdecl GfxParticleRender_RenderParticleClumps(int /*atomic*/, int emitterAddress)
{
    auto* emitter = reinterpret_cast<std::uint8_t*>(emitterAddress);

    GetGlobalValue_6C5760();
    const int clumpOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, reinterpret_cast<void*>(CSF_AT(int, emitter, 0xC)), 0x10), 10);
    RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, reinterpret_cast<void*>(CSF_AT(int, emitter, 0xC)), 0x10), 11);
    const int tankOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, reinterpret_cast<void*>(CSF_AT(int, emitter, 0xC)), 0x10), 6);

    auto* clumpProperty = clumpOffset == -1 ? nullptr : emitter + clumpOffset;
    auto* tankProperty = tankOffset == -1 ? nullptr : emitter + tankOffset;

    alignas(8) std::uint8_t lockBytes[0x8];
    auto* lock = reinterpret_cast<std::uint32_t*>(lockBytes);
    RpPTankAtomicLock(CSF_AT(int, tankProperty, 0x18), lock, 8, 0x40000000);

    const int clump = CSF_AT(
        int, reinterpret_cast<void*>(CSF_AT(int, clumpProperty, 0)), 0x4);
    int frame = 0;
    if (clump)
        frame = CSF_AT(int, reinterpret_cast<void*>(clump), 0x4);

    alignas(8) std::uint8_t angleBytes[0xC];
    auto* angles = reinterpret_cast<float*>(angleBytes);
    alignas(8) std::uint8_t sourceMatrixBytes[0x8];
    alignas(8) std::uint8_t transformedMatrixBytes[0x8];
    alignas(8) std::uint8_t state8Bytes[0x4] = {};
    alignas(8) std::uint8_t state6Bytes[0x4] = {};
    alignas(8) std::uint8_t state20Bytes[0xC] = {};

    if (CSF_AT(std::uint32_t, tankProperty, 0x8))
    {
        std::uint32_t particleIndex = 0;
        do
        {
            if (clump)
            {
                if (frame)
                {
                    CSF_AT(std::uint32_t, sourceMatrixBytes, 0) = lock[0];
                    CSF_AT(std::uint8_t, sourceMatrixBytes, 0x4) = 0;
                    GfxMatrix_GetEulerAngles(
                        reinterpret_cast<float**>(sourceMatrixBytes), nullptr, angles);

                    const auto* translation = reinterpret_cast<const std::uint32_t*>(
                        reinterpret_cast<const std::uint8_t*>(
                            CSF_AT(std::uint32_t*, sourceMatrixBytes, 0)) + 48);

                    CSF_AT(std::uint32_t*, transformedMatrixBytes, 0) = RwMatrixCreate();
                    CSF_AT(std::uint8_t, transformedMatrixBytes, 0x4) = 1;
                    GfxMatrix_RotateAroundAxisX(
                        reinterpret_cast<int*>(transformedMatrixBytes), nullptr, angles[0]);
                    GfxMatrix_RotateAroundAxisY(
                        reinterpret_cast<int*>(transformedMatrixBytes), nullptr, angles[1]);
                    GfxMatrix_RotateAroundAxisZ(
                        reinterpret_cast<int*>(transformedMatrixBytes), nullptr, angles[2]);
                    GfxMatrix_CopyTranslationAndUpdate(
                        transformedMatrixBytes, nullptr, translation);
                    RwFrameTransform(
                        frame,
                        reinterpret_cast<int>(CSF_AT(std::uint32_t*, transformedMatrixBytes, 0)),
                        0);

                    if (CSF_AT(std::uint8_t, transformedMatrixBytes, 0x4))
                        RwMatrixDestroy(CSF_AT(std::uint32_t*, transformedMatrixBytes, 0));
                    if (CSF_AT(std::uint8_t, sourceMatrixBytes, 0x4))
                        RwMatrixDestroy(CSF_AT(std::uint32_t*, sourceMatrixBytes, 0));
                }

                reinterpret_cast<void (__cdecl**)(int, int*)>(g_pRwEngineInstance)[9](
                    20, reinterpret_cast<int*>(state20Bytes));
                reinterpret_cast<void (__cdecl**)(int, int*)>(g_pRwEngineInstance)[9](
                    6, reinterpret_cast<int*>(state6Bytes));
                reinterpret_cast<void (__cdecl**)(int, int*)>(g_pRwEngineInstance)[9](
                    8, reinterpret_cast<int*>(state8Bytes));

                reinterpret_cast<void (__cdecl**)(int, int)>(g_pRwEngineInstance)[8](20, 2);
                reinterpret_cast<void (__cdecl**)(int, int)>(g_pRwEngineInstance)[8](6, 1);
                reinterpret_cast<void (__cdecl**)(int, int)>(g_pRwEngineInstance)[8](8, 1);

                RpClumpRender(clump);

                reinterpret_cast<void (__cdecl**)(int, int)>(g_pRwEngineInstance)[8](
                    20, CSF_AT(int, state20Bytes, 0));
                reinterpret_cast<void (__cdecl**)(int, int)>(g_pRwEngineInstance)[8](
                    6, CSF_AT(int, state6Bytes, 0));
                reinterpret_cast<void (__cdecl**)(int, int)>(g_pRwEngineInstance)[8](
                    8, CSF_AT(int, state8Bytes, 0));
            }

            lock[0] += lock[1];
            const std::uint32_t particleCount = CSF_AT(std::uint32_t, tankProperty, 0x8);
            ++particleIndex;
            if (particleIndex >= particleCount)
                break;
        } while (true);
    }

    RpPTankAtomicUnlock(CSF_AT(int, tankProperty, 0x18));
    return emitterAddress;
}

HOOK(0x00704000, GfxParticleRender_RenderParticleClumps, "auto");
