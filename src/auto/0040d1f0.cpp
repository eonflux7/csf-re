#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0,
         (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __fastcall, S_006ada10_SetRotation, 0x006AE2F0,
         (S_006ada10* self, void* , float angleX, float angleY, float angleZ));

CSF_GLOBAL(float, g_MaximumHeadTimeStep, 0x0082DC68);
CSF_GLOBAL(float, g_SwayRollAmplitude, 0x0082DC6C);
CSF_GLOBAL(float, g_SubtitlePanelScale, 0x0082DC70);
CSF_GLOBAL(float, g_SwayResponse, 0x0082DC74);
CSF_GLOBAL(float, g_SwayImpulseScale, 0x0082DC78);
CSF_GLOBAL(float, g_DirectionYScale, 0x0082DC7C);
CSF_GLOBAL(float, g_SwayHeightScale, 0x0082DC80);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_SmoothedSwayHeight, 0x008AFC6C);

static char __fastcall UpdateViewSway(void* self, void* /*edx*/, float* elapsed)
{
    alignas(8) std::uint8_t matrixBytes[0x14];
    auto& matrix = *reinterpret_cast<S_006ada10*>(matrixBytes);
    auto& ownsMatrix = CSF_AT(char, matrixBytes, 0x04);
    auto& heightOffset = CSF_AT(float, matrixBytes, 0x08);
    auto& pitchPhase = CSF_AT(float, matrixBytes, 0x0C);
    auto& rollOffset = CSF_AT(float, matrixBytes, 0x10);

    void* owner = CSF_AT(void*, self, 0x04);
    void* heightSource = CSF_AT(void*, owner, 0x30);
    g_SmoothedSwayHeight =
        (CSF_AT(float, heightSource, 0x50) - g_SmoothedSwayHeight) *
        g_flt_0082DC84 + g_SmoothedSwayHeight;
    heightOffset = g_SmoothedSwayHeight / CSF_AT(float, owner, 0x184) *
                   g_SwayHeightScale;

    const float previousRoll = CSF_AT(float, self, 0x4C);
    const float impulse = CSF_AT(float, self, 0x48);
    CSF_AT(std::uint32_t, self, 0x48) = 0;
    rollOffset = previousRoll +
        (impulse * g_DirectionYScale * g_SwayImpulseScale - previousRoll) *
        g_SwayResponse;
    CSF_AT(float, self, 0x4C) = rollOffset;

    CSF_AT(float, self, 0x3C) =
        *elapsed * g_SubtitlePanelScale + CSF_AT(float, self, 0x3C);
    const float rollPhase = CSF_AT(float, self, 0x3C);
    const float step = *elapsed;
    CSF_AT(float, self, 0x40) =
        step + step + CSF_AT(float, self, 0x40);
    pitchPhase = CSF_AT(float, self, 0x40);

    void* view = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

    matrix.field_0000 = reinterpret_cast<std::uint32_t>(RwMatrixCreate());
    ownsMatrix = 1;
    const float yaw = CSF_AT(float, self, 0x44);
    const float angleZ = std::sin(rollPhase) * g_SwayRollAmplitude + rollOffset;
    const float angleX = std::sin(pitchPhase) * g_MaximumHeadTimeStep + heightOffset;
    S_006ada10_SetRotation(&matrix, nullptr, angleX, yaw, angleZ);

    const std::uint32_t* position =
        GetObjectPositionVector(CSF_AT(void*, self, 0x04), nullptr);
    GfxMatrix_CopyTranslationAndUpdate(&matrix, nullptr, position);

    void* transform = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, view, 0)[0x18 / 4])(view, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*, S_006ada10*)>(
        CSF_AT(void**, transform, 0)[0x94 / 4])(transform, nullptr, &matrix);

    const char owned = ownsMatrix;
    if (owned)
        return static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(matrix.field_0000)));
    return owned;
}

HOOK(0x0040D1F0, UpdateViewSway, "auto");
