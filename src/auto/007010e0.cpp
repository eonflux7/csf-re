#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, SceneLoader_CreateIndexedGeometry, 0x006B1DE0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_AddColoredVertex, 0x006C3630, (void* self, void* edx, uint32_t* a2, uint8_t* a3));
CSF_FUNC(int, __fastcall, SceneLoader_AddOutlineIndex, 0x007387E0, (void* self, void* edx, int16_t a2));

static int __fastcall S_007010e0_BuildOutlineGeometry(
    S_007010e0* self, void* /*edx*/)
{
    alignas(8) uint8_t secondVertexBytes[0xC];
    alignas(8) uint8_t firstVertexBytes[0x10];
    alignas(8) uint8_t colorBytes[0x4];

    void* contour = C_008310b8_GetSelf(
        reinterpret_cast<C_008310b8*>(self->field_0004), nullptr);

    const uint32_t vertexCount = 2 * CSF_AT(uint32_t, contour, 0x8);
    CSF_AT(uint32_t, firstVertexBytes, 0xC) = vertexCount;

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
    void* geometry = SceneLoader_CreateIndexedGeometry(
        sceneLoader, nullptr,
        static_cast<int>(vertexCount),
        static_cast<int>(vertexCount + 2));

    self->field_0028 = reinterpret_cast<uint32_t>(geometry);
    CSF_AT(uint8_t, geometry, 0x20) = 0;
    reinterpret_cast<C_00867358*>(self->field_0028)->field_001c = 4;

    CSF_AT(uint32_t, colorBytes, 0) = 0x140000FF;

    for (uint32_t i = 0; i < CSF_AT(uint32_t, contour, 0x8); ++i)
    {
        void* points = CSF_AT(void*, contour, 0);
        const uint32_t firstX = CSF_AT(uint32_t, points, 8 * i);
        CSF_AT(float, firstVertexBytes, 0x8) =
            CSF_AT(float, points, 8 * i + 4);
        const uint32_t firstY = self->field_0008;
        CSF_AT(uint32_t, firstVertexBytes, 0) = firstX;
        CSF_AT(uint32_t, firstVertexBytes, 0x4) = firstY;

        SceneLoader_AddColoredVertex(
            reinterpret_cast<void*>(self->field_0028), nullptr,
            reinterpret_cast<uint32_t*>(firstVertexBytes), colorBytes);

        points = CSF_AT(void*, contour, 0);
        const uint32_t secondX = CSF_AT(uint32_t, points, 8 * i);
        const float secondZ = CSF_AT(float, points, 8 * i + 4);
        const uint32_t secondY = self->field_000c;
        CSF_AT(float, secondVertexBytes, 0x8) = secondZ;
        CSF_AT(uint32_t, secondVertexBytes, 0) = secondX;
        CSF_AT(uint32_t, secondVertexBytes, 0x4) = secondY;

        SceneLoader_AddColoredVertex(
            reinterpret_cast<void*>(self->field_0028), nullptr,
            reinterpret_cast<uint32_t*>(secondVertexBytes), colorBytes);
    }

    const uint32_t outlineVertexCount =
        CSF_AT(uint32_t, firstVertexBytes, 0xC);
    for (uint32_t i = 0; i < outlineVertexCount; ++i)
    {
        SceneLoader_AddOutlineIndex(
            reinterpret_cast<void*>(self->field_0028), nullptr,
            static_cast<int16_t>(i));
    }

    SceneLoader_AddOutlineIndex(
        reinterpret_cast<void*>(self->field_0028), nullptr, 0);
    return SceneLoader_AddOutlineIndex(
        reinterpret_cast<void*>(self->field_0028), nullptr, 1);
}

HOOK(0x007010E0, S_007010e0_BuildOutlineGeometry, "auto");
