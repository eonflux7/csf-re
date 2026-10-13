#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(uint32_t*, __fastcall, CHud_CopyBoundsFromSource, 0x0042E150, (CHud* self, void* , int* a2));
CSF_FUNC(int, __fastcall, CHud_AttachPanelsToTarget, 0x0042E1A0, (CHud* self, void* , int a2));
CSF_FUNC(int, __fastcall, CHud_LookupValueByKey, 0x00431F10, (CHud* self, void* , int key));
CSF_FUNC(double, __fastcall, CGuiTacticalMap_GetPositionRotation, 0x004369D0, (void* self, void* edx, float* a2));
CSF_FUNC(char, __fastcall, CGuiTacticalMap_AdjustMapPosition, 0x00436C90, (void* self, void* edx, float* a2));
CSF_FUNC(uint32_t*, __fastcall, CGuiTacticalMap_ProjectMapPosition, 0x00436D30, (void* self, void* edx, uint32_t* a2, float* a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_Scale, 0x006ADB60, (void* self, void* , int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, GfxMatrix_Translate, 0x006ADBB0, (void* self, void* , int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisZ, 0x006AE210, (int* self, void* , float angle));

static int HudSpritesLayout_FloatArgumentBits(float value)
{
    int bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static int* __fastcall CGuiTacticalMap_UpdatePositionMarker(
    CGuiTacticalMap* self, void* /*edx*/, float* worldPosition)
{
    CHud* hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
    int* marker = reinterpret_cast<int*>(CHud_LookupValueByKey(hud, nullptr, 11));
    if (!marker)
        return marker;

    alignas(8) std::uint8_t positionBytes[0xC];
    alignas(8) std::uint8_t pointBytes[0x8];
    alignas(8) std::uint8_t screenSizeBytes[0x8];
    alignas(8) std::uint8_t matrixBytes[0xC];

    float* mapPosition = reinterpret_cast<float*>(positionBytes);
    std::memcpy(mapPosition, worldPosition, 0xC);
    CGuiTacticalMap_AdjustMapPosition(self, nullptr, mapPosition);
    CGuiTacticalMap_ProjectMapPosition(
        self, nullptr, reinterpret_cast<uint32_t*>(pointBytes), mapPosition);

    hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
    CHud_CopyBoundsFromSource(hud, nullptr, marker);
    reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, marker, 0)[0x28 / 4])(
            marker, nullptr, reinterpret_cast<int*>(pointBytes));

    CSF_AT(float, matrixBytes, 0x8) =
        static_cast<float>(CGuiTacticalMap_GetPositionRotation(self, nullptr, worldPosition));
    CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, matrixBytes, 0x4) = 1;

    std::uint32_t* matrix = CSF_AT(std::uint32_t*, matrixBytes, 0);
    CSF_AT(float, matrix, 0x28) = 1.0f;
    CSF_AT(float, matrix, 0x14) = 1.0f;
    CSF_AT(float, matrix, 0x00) = 1.0f;
    CSF_AT(float, matrix, 0x10) = 0.0f;
    CSF_AT(float, matrix, 0x08) = 0.0f;
    CSF_AT(float, matrix, 0x04) = 0.0f;
    CSF_AT(float, matrix, 0x24) = 0.0f;
    CSF_AT(float, matrix, 0x20) = 0.0f;
    CSF_AT(float, matrix, 0x18) = 0.0f;
    CSF_AT(float, matrix, 0x38) = 0.0f;
    CSF_AT(float, matrix, 0x34) = 0.0f;
    CSF_AT(float, matrix, 0x30) = 0.0f;
    CSF_AT(std::uint32_t, matrix, 0x0C) |= 0x20003u;

    CSF_AT(float, screenSizeBytes, 0) = 0.0f;
    CSF_AT(float, screenSizeBytes, 0x4) = 0.0f;
    Hud_WriteNormalizedScreenSize(reinterpret_cast<float*>(screenSizeBytes));

    const float negativeX = static_cast<float>(static_cast<std::int32_t>(
        0u - CSF_AT(std::uint32_t, pointBytes, 0)));
    const float negativeY = static_cast<float>(static_cast<std::int32_t>(
        0u - CSF_AT(std::uint32_t, pointBytes, 0x4)));
    GfxMatrix_Translate(
        matrixBytes, nullptr,
        HudSpritesLayout_FloatArgumentBits(negativeX),
        HudSpritesLayout_FloatArgumentBits(negativeY), 0);

    GfxMatrix_RotateAroundAxisZ(
        reinterpret_cast<int*>(matrixBytes), nullptr,
        CSF_AT(float, matrixBytes, 0x8));

    GfxMatrix_Scale(
        matrixBytes, nullptr,
        HudSpritesLayout_FloatArgumentBits(CSF_AT(float, screenSizeBytes, 0)),
        HudSpritesLayout_FloatArgumentBits(CSF_AT(float, screenSizeBytes, 0x4)), 0);

    const float translatedX =
        static_cast<float>(CSF_AT(std::int32_t, pointBytes, 0)) *
        CSF_AT(float, screenSizeBytes, 0);
    const float translatedY =
        static_cast<float>(CSF_AT(std::int32_t, pointBytes, 0x4)) *
        CSF_AT(float, screenSizeBytes, 0x4);
    GfxMatrix_Translate(
        matrixBytes, nullptr,
        HudSpritesLayout_FloatArgumentBits(translatedX),
        HudSpritesLayout_FloatArgumentBits(translatedY), 0);

    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, marker, 0)[0x60 / 4])(marker, nullptr, matrixBytes);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, marker, 0)[0x8 / 4])(marker, nullptr);

    hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
    int result = CHud_AttachPanelsToTarget(hud, nullptr, reinterpret_cast<int>(marker));
    if (CSF_AT(std::uint8_t, matrixBytes, 0x4))
        result = RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

    return reinterpret_cast<int*>(result);
}

HOOK(0x00437390, CGuiTacticalMap_UpdatePositionMarker, "auto");
