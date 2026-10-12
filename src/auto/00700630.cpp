#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxSkin_CombineTransform, 0x0048E060, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(uint32_t*, __cdecl, RwMatrixMultiply, 0x00624630, (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(std::uint32_t*, __fastcall, GfxMatrix_TransformBasisAndPosition, 0x006ABF10, (std::uint32_t* self, void* , std::uint32_t* matrix));
CSF_FUNC(char, __fastcall, GfxTeam_DrawStringMarker, 0x006C3990, (void* self, void* edx, float* a2, float a3, uint8_t* a4));
CSF_FUNC(int, __fastcall, GfxTeam_DrawBounds, 0x006C3E60, (void* self, void* edx, void* a2, uint8_t* a3));
CSF_FUNC(void*, __stdcall, GfxTeam_GetObjectBounds, 0x00700250, (void* a1, int arg4));
CSF_FUNC(double, __cdecl, Rt2dObjectStringGetHeight, 0x007F9880, (int a1));
CSF_FUNC(int, __cdecl, Rt2dSceneGetChildCount, 0x008013C0, (int a1));
CSF_FUNC(int, __cdecl, GfxTeam_GetSceneChild, 0x008013D0, (uint32_t* a1, int a2));
CSF_FUNC(int, __cdecl, Rt2dClumpGetClump, 0x00801520, (int a1));

static char __fastcall S_006ff7e0_DrawObjectBounds(
    S_006ff7e0* self, void* /*edx*/, int objectAddress, int playerIndex)
{
    alignas(8) std::uint8_t boundsBytes[0x3C];
    alignas(8) std::uint8_t firstMatrixBytes[0x8];
    alignas(8) std::uint8_t objectMatrixBytes[0x8];
    alignas(8) std::uint8_t finalMatrixBytes[0x8];

    auto* object = reinterpret_cast<void*>(objectAddress);
    const std::uint16_t objectType =
        CSF_AT(std::uint16_t, CSF_AT(void*, object, 0x5C), 0x00);

    switch (objectType)
    {
    case 5:
    {
        GfxTeam_GetObjectBounds(boundsBytes, objectAddress);

        auto* playerMatrix = CSF_AT(
            std::uint32_t*,
            CSF_AT(void*, reinterpret_cast<void*>(self->field_0004), 0x10),
            12 * static_cast<std::uint32_t>(playerIndex) + 8);
        CSF_AT(std::uint32_t*, firstMatrixBytes, 0x00) =
            (CSF_AT(std::uint32_t, CSF_AT(void*, object, 0x5C), 0x40) & 2)
                ? nullptr : reinterpret_cast<std::uint32_t*>(object);

        auto* temporaryMatrix = RwMatrixCreate();
        CSF_AT(std::uint32_t*, objectMatrixBytes, 0x00) = temporaryMatrix;
        RwMatrixMultiply(
            temporaryMatrix,
            CSF_AT(std::uint32_t*, firstMatrixBytes, 0x00),
            playerMatrix);

        auto* copiedMatrix = RwMatrixCreate();
        CSF_AT(std::uint32_t*, finalMatrixBytes, 0x00) = copiedMatrix;
        std::memcpy(copiedMatrix, temporaryMatrix, 0x40);
        CSF_AT(std::uint8_t, finalMatrixBytes, 0x04) = 1;
        RwMatrixDestroy(CSF_AT(std::uint32_t*, objectMatrixBytes, 0x00));

        GfxMatrix_TransformBasisAndPosition(
            reinterpret_cast<std::uint32_t*>(boundsBytes), nullptr,
            reinterpret_cast<std::uint32_t*>(finalMatrixBytes));
        if (CSF_AT(std::uint8_t, finalMatrixBytes, 0x04))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, finalMatrixBytes, 0x00));

        int color = -16776961;
        return static_cast<char>(GfxTeam_DrawBounds(
            reinterpret_cast<void*>(self->field_0024), nullptr, boundsBytes,
            reinterpret_cast<std::uint8_t*>(&color)));
    }

    case 2:
    {
        const float height =
            static_cast<float>(Rt2dObjectStringGetHeight(objectAddress));
        auto* playerMatrix = CSF_AT(
            std::uint32_t*,
            CSF_AT(void*, reinterpret_cast<void*>(self->field_0004), 0x10),
            12 * static_cast<std::uint32_t>(playerIndex) + 8);
        const bool omitObjectMatrix =
            (CSF_AT(std::uint32_t, CSF_AT(void*, object, 0x5C), 0x40) & 2) != 0;

        int color = -16776961;
        CSF_AT(std::uint32_t*, objectMatrixBytes, 0x00) =
            omitObjectMatrix ? nullptr : reinterpret_cast<std::uint32_t*>(object);
        CSF_AT(std::uint32_t*, finalMatrixBytes, 0x00) = playerMatrix;
        CSF_AT(std::uint8_t, finalMatrixBytes, 0x04) = 0;
        CSF_AT(std::uint8_t, objectMatrixBytes, 0x04) = 0;

        const int combined = GfxSkin_CombineTransform(
            objectMatrixBytes, nullptr, reinterpret_cast<int>(firstMatrixBytes),
            reinterpret_cast<std::uint32_t*>(finalMatrixBytes));
        auto* position = reinterpret_cast<float*>(
            reinterpret_cast<std::uint8_t*>(
                CSF_AT(void*, reinterpret_cast<void*>(combined), 0x00)) + 0x30);
        GfxTeam_DrawStringMarker(
            reinterpret_cast<void*>(self->field_0024), nullptr, position, height,
            reinterpret_cast<std::uint8_t*>(&color));

        const char ownsMatrix = CSF_AT(char, firstMatrixBytes, 0x04);
        if (ownsMatrix)
            return static_cast<char>(
                RwMatrixDestroy(CSF_AT(std::uint32_t*, firstMatrixBytes, 0x00)));
        return ownsMatrix;
    }

    case 3:
    case 6:
    {
        GfxTeam_GetObjectBounds(boundsBytes, objectAddress);

        auto* playerMatrix = CSF_AT(
            std::uint32_t*,
            CSF_AT(void*, reinterpret_cast<void*>(self->field_0004), 0x10),
            12 * static_cast<std::uint32_t>(playerIndex) + 8);
        const bool omitObjectMatrix =
            (CSF_AT(std::uint32_t, CSF_AT(void*, object, 0x5C), 0x40) & 2) != 0;

        CSF_AT(std::uint32_t*, firstMatrixBytes, 0x00) = playerMatrix;
        CSF_AT(std::uint8_t, firstMatrixBytes, 0x04) = 0;
        CSF_AT(std::uint32_t*, objectMatrixBytes, 0x00) =
            omitObjectMatrix ? nullptr : reinterpret_cast<std::uint32_t*>(object);
        CSF_AT(std::uint8_t, objectMatrixBytes, 0x04) = 0;

        const int combined = GfxSkin_CombineTransform(
            objectMatrixBytes, nullptr, reinterpret_cast<int>(finalMatrixBytes),
            reinterpret_cast<std::uint32_t*>(firstMatrixBytes));
        GfxMatrix_TransformBasisAndPosition(
            reinterpret_cast<std::uint32_t*>(boundsBytes), nullptr,
            reinterpret_cast<std::uint32_t*>(combined));

        if (CSF_AT(std::uint8_t, finalMatrixBytes, 0x04))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, finalMatrixBytes, 0x00));

        int color = -10197761;
        return static_cast<char>(GfxTeam_DrawBounds(
            reinterpret_cast<void*>(self->field_0024), nullptr, boundsBytes,
            reinterpret_cast<std::uint8_t*>(&color)));
    }

    case 8:
    {
        const int clump = Rt2dClumpGetClump(objectAddress);
        const std::uint32_t childCount =
            static_cast<std::uint32_t>(Rt2dSceneGetChildCount(clump));
        for (std::uint32_t childIndex = 0; childIndex < childCount; ++childIndex)
        {
            const int child = GfxTeam_GetSceneChild(
                reinterpret_cast<std::uint32_t*>(clump),
                static_cast<int>(childIndex));
            S_006ff7e0_DrawObjectBounds(self, nullptr, child, playerIndex);
        }
        return static_cast<char>(childCount);
    }

    default:
        return static_cast<char>(objectType);
    }
}

HOOK(0x00700630, S_006ff7e0_DrawObjectBounds, "auto");
