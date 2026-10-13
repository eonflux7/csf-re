#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(float*, __fastcall, S_006abb80_IncludePointInBounds, 0x006AC6D0, (S_006abb80* self, void* , float* point));
CSF_FUNC(int, __fastcall, GfxMatrix_ExtractAndApplyTransform, 0x006ADF00, (S_006ada10* self, void* , void* sourceData));
CSF_FUNC(int, __fastcall, GfxRenderEngine_ApplyParticleTransform, 0x006EF1E0, (void* self, void* edx, void** a2));

static char __fastcall GfxRenderEngine_SetParticleEffectEndpoints(
    void* self, void* /*edx*/, float* start, float* end)
{
    auto* objectBytes = static_cast<std::uint8_t*>(self);

    CSF_AT(float, self, 0x5C) = start[0];
    CSF_AT(float, self, 0x60) = start[1];
    CSF_AT(float, self, 0x64) = start[2];
    CSF_AT(float, self, 0x68) = end[0];
    CSF_AT(float, self, 0x6C) = end[1];
    CSF_AT(float, self, 0x70) = end[2];
    CSF_AT(float, self, 0x74) = start[0];
    CSF_AT(float, self, 0x78) = start[1];
    CSF_AT(float, self, 0x7C) = start[2];

    // The converted direction shares its last eight bytes with the
    // matrix wrapper, which is constructed only after that result is copied.
    alignas(8) std::uint8_t scratch[0x18];
    CSF_AT(float, scratch, 0x00) =
        CSF_AT(float, self, 0x68) - CSF_AT(float, self, 0x5C);
    CSF_AT(float, scratch, 0x04) =
        CSF_AT(float, self, 0x6C) - CSF_AT(float, self, 0x60);
    CSF_AT(float, scratch, 0x08) =
        CSF_AT(float, self, 0x70) - CSF_AT(float, self, 0x64);

    auto* convertedDirection = S_00526d40_ConvertDirection(
        reinterpret_cast<S_00526d40*>(scratch), nullptr,
        reinterpret_cast<std::uint32_t*>(scratch + 0x0C));

    CSF_AT(std::uint32_t, self, 0x88) = convertedDirection[0];
    CSF_AT(std::uint32_t, self, 0x8C) = convertedDirection[1];
    CSF_AT(std::uint32_t, self, 0x90) = convertedDirection[2];

    CSF_AT(std::uint32_t, self, 0x80) = 0;
    CSF_AT(float, self, 0x84) = std::sqrt(
        CSF_AT(float, scratch, 0x08) * CSF_AT(float, scratch, 0x08) +
        CSF_AT(float, scratch, 0x04) * CSF_AT(float, scratch, 0x04) +
        CSF_AT(float, scratch, 0x00) * CSF_AT(float, scratch, 0x00));

    auto* bounds = reinterpret_cast<S_006abb80*>(objectBytes + 0x20);
    S_006abb80_IncludePointInBounds(bounds, nullptr, start);
    S_006abb80_IncludePointInBounds(bounds, nullptr, end);

    if (!CSF_AT(void*, self, 0x94))
        return 0;

    CSF_AT(std::uint32_t*, scratch, 0x10) = RwMatrixCreate();
    CSF_AT(char, scratch, 0x14) = 1;

    GfxMatrix_ExtractAndApplyTransform(
        reinterpret_cast<S_006ada10*>(scratch + 0x10), nullptr,
        objectBytes + 0x88);
    GfxRenderEngine_ApplyParticleTransform(
        CSF_AT(void*, self, 0x94), nullptr,
        reinterpret_cast<void**>(scratch + 0x10));

    void* effect = CSF_AT(void*, self, 0x94);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, effect, 0)[0x20 / 4])(
            effect, nullptr, objectBytes + 0x5C);

    effect = CSF_AT(void*, self, 0x94);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, effect, 0)[0x2C / 4])(effect, nullptr, 0);

    char ownsMatrix = CSF_AT(char, scratch, 0x14);
    if (ownsMatrix)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, scratch, 0x10)));
    return ownsMatrix;
}

HOOK(0x006DCAF0, GfxRenderEngine_SetParticleEffectEndpoints, "auto");
