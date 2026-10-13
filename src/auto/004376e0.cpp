#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(uint32_t*, __fastcall, CGuiTacticalMap_ComputeSpritePosition, 0x00436D30, (void* self, void* edx, uint32_t* a2, float* a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_Scale, 0x006ADB60, (void* self, void* , int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, GfxMatrix_Translate, 0x006ADBB0, (void* self, void* , int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisZ, 0x006AE210, (int* self, void* , float angle));

CSF_GLOBAL(float, g_TacticalMapDegreesToRadians, 0x00830A14);

static int EncodeFloatArgument(float value)
{
    int bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static void __fastcall CGuiTacticalMap_ApplySpriteTransform(
    CGuiTacticalMap* self, void* /*edx*/, int* sprite, float* position,
    float angleDegrees)
{
    if (!sprite)
        return;

    alignas(8) std::uint8_t positionBytes[0x8];
    alignas(8) std::uint8_t screenSizeBytes[0x8];
    alignas(8) std::uint8_t matrixBytes[0x8];

    void** initialVtable = CSF_AT(void**, sprite, 0);
    uint32_t* spritePosition = CGuiTacticalMap_ComputeSpritePosition(
        self, nullptr, reinterpret_cast<uint32_t*>(positionBytes), position);

    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
        initialVtable[0x28 / 4])(sprite, nullptr, spritePosition);

    int* pivot = reinterpret_cast<int* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, sprite, 0)[0x2C / 4])(sprite, nullptr);

    CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, matrixBytes, 0x4) = 1;

    std::uint32_t* matrix = CSF_AT(std::uint32_t*, matrixBytes, 0);
    CSF_AT(std::uint32_t, matrix, 0x28) = 1065353216;
    CSF_AT(std::uint32_t, matrix, 0x14) = 1065353216;
    CSF_AT(std::uint32_t, matrix, 0x00) = 1065353216;
    CSF_AT(std::uint32_t, matrix, 0x10) = 0;
    CSF_AT(std::uint32_t, matrix, 0x08) = 0;
    CSF_AT(std::uint32_t, matrix, 0x04) = 0;
    CSF_AT(std::uint32_t, matrix, 0x24) = 0;
    CSF_AT(std::uint32_t, matrix, 0x20) = 0;
    CSF_AT(std::uint32_t, matrix, 0x18) = 0;
    CSF_AT(std::uint32_t, matrix, 0x38) = 0;
    CSF_AT(std::uint32_t, matrix, 0x34) = 0;
    CSF_AT(std::uint32_t, matrix, 0x30) = 0;
    CSF_AT(std::uint32_t, matrix, 0x0C) |= 0x20003u;

    CSF_AT(float, screenSizeBytes, 0) = 0.0f;
    CSF_AT(float, screenSizeBytes, 0x4) = 0.0f;
    Hud_WriteNormalizedScreenSize(reinterpret_cast<float*>(screenSizeBytes));

    const float negativeY = static_cast<float>(
        static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(pivot[1])));
    const float negativeX = static_cast<float>(
        static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(pivot[0])));
    GfxMatrix_Translate(matrixBytes, nullptr,
        EncodeFloatArgument(negativeX), EncodeFloatArgument(negativeY), 0);

    GfxMatrix_RotateAroundAxisZ(reinterpret_cast<int*>(matrixBytes), nullptr,
        -angleDegrees * g_TacticalMapDegreesToRadians);

    GfxMatrix_Scale(matrixBytes, nullptr,
        EncodeFloatArgument(CSF_AT(float, screenSizeBytes, 0)),
        EncodeFloatArgument(CSF_AT(float, screenSizeBytes, 0x4)), 0);

    const float scaledY =
        static_cast<float>(pivot[1]) * CSF_AT(float, screenSizeBytes, 0x4);
    const float scaledX =
        static_cast<float>(pivot[0]) * CSF_AT(float, screenSizeBytes, 0);
    GfxMatrix_Translate(matrixBytes, nullptr,
        EncodeFloatArgument(scaledX), EncodeFloatArgument(scaledY), 0);

    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t**)>(
        CSF_AT(void**, sprite, 0)[0x60 / 4])(
            sprite, nullptr, reinterpret_cast<std::uint32_t**>(matrixBytes));

    if (CSF_AT(std::uint8_t, matrixBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));
}

HOOK(0x004376E0, CGuiTacticalMap_ApplySpriteTransform, "auto");
