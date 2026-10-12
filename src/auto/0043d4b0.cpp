#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// ---- bindings -------------------------------------------------------------------------------
CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angleRadians));
CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_Scale, 0x006ADB60, (void* self, void* , int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, GfxMatrix_Translate, 0x006ADBB0, (void* self, void* , int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisZ, 0x006AE210, (int* self, void* , float angle));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);   // 0.5f
CSF_GLOBAL(float, g_RadarDegToRad, 0x00830E58);       // pi / 180

// Scoped RenderWare matrix as the original keeps it on its stack: { RwMatrix*, bool owned }.
struct RadarMatrixHolder
{
    std::uint32_t* matrix;
    std::uint8_t   owned;
};

// The GfxMatrix_* helpers take their float components as raw 32-bit stack slots.
static int FloatToStackSlot(float value)
{
    int slot;
    std::memcpy(&slot, &value, sizeof(slot));
    return slot;
}

// Geometry builder virtual: append one triangle index (vtable +0x38).
static void RadarBuilder_AddIndex(C_00830e5c* self, int index)
{
    void* builder = reinterpret_cast<void*>(self->field_001c);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, builder, 0)[0x38 / 4])(builder, nullptr, index);
}

// Rebuilds the multiplayer radar disc: transforms the stored rim points by the radar rotation and
// the screen scale, feeds them to the geometry builder and emits the triangle indices.
static char __fastcall C_00830e5c_RebuildRadarGeometry(C_00830e5c* self, void* /*edx*/)
{
    void* builder = reinterpret_cast<void*>(self->field_001c);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, builder, 0)[0x24 / 4])(builder, nullptr);

    builder = reinterpret_cast<void*>(self->field_001c);
    if (!((int(__fastcall*)(void*, void*))CSF_AT(void**, builder, 0)[0x3C / 4])(builder, nullptr))
    {
        const std::uint32_t segmentCount = self->field_0020;
        builder = reinterpret_cast<void*>(self->field_001c);
        ((void(__fastcall*)(void*, void*, std::uint32_t, std::uint32_t))CSF_AT(void**, builder, 0)[0x2C / 4])(
            builder, nullptr, 2 * segmentCount + 1, 9 * segmentCount);

        builder = reinterpret_cast<void*>(self->field_001c);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, builder, 0)[0x20 / 4])(builder, nullptr, 1);
    }

    alignas(8) std::uint8_t holderBytes[8];
    RadarMatrixHolder& holder = *reinterpret_cast<RadarMatrixHolder*>(holderBytes);
    holder.matrix = RwMatrixCreate();
    holder.owned = 1;

    // Identity matrix.
    CSF_AT(float, holder.matrix, 0x28) = 1.0f;
    CSF_AT(float, holder.matrix, 0x14) = 1.0f;
    CSF_AT(float, holder.matrix, 0x00) = 1.0f;
    CSF_AT(float, holder.matrix, 0x10) = 0.0f;
    CSF_AT(float, holder.matrix, 0x08) = 0.0f;
    CSF_AT(float, holder.matrix, 0x04) = 0.0f;
    CSF_AT(float, holder.matrix, 0x24) = 0.0f;
    CSF_AT(float, holder.matrix, 0x20) = 0.0f;
    CSF_AT(float, holder.matrix, 0x18) = 0.0f;
    CSF_AT(float, holder.matrix, 0x38) = 0.0f;
    CSF_AT(float, holder.matrix, 0x34) = 0.0f;
    CSF_AT(float, holder.matrix, 0x30) = 0.0f;
    CSF_AT(std::uint32_t, holder.matrix, 0x0C) |= 0x20003u;

    // Centre of the radar rectangle (left, top, right, bottom at +0x24..+0x30).
    const int top = static_cast<int>(self->field_0028);
    const int height = static_cast<int>(self->field_0030 - self->field_0028);
    const int left = static_cast<int>(self->field_0024);
    const int width = static_cast<int>(self->field_002c - self->field_0024);

    const float centerX = static_cast<float>(width) * g_HalfDurationScale + static_cast<float>(left);
    const float centerY = static_cast<float>(height) * g_HalfDurationScale + static_cast<float>(top);

    GfxMatrix_Translate(&holder, nullptr, FloatToStackSlot(-centerX), FloatToStackSlot(-centerY), 0);

    const float angle = static_cast<float>(MenuMpCharacterSelect_NormalizeAngle(self->field_0048 * g_RadarDegToRad));
    GfxMatrix_RotateAroundAxisZ(reinterpret_cast<int*>(&holder), nullptr, angle);

    float screenScale[2];
    screenScale[0] = 0.0f;
    screenScale[1] = 0.0f;
    Hud_WriteNormalizedScreenSize(screenScale);

    GfxMatrix_Scale(&holder, nullptr, FloatToStackSlot(screenScale[0]), FloatToStackSlot(screenScale[1]), 0);
    GfxMatrix_Translate(&holder, nullptr,
                        FloatToStackSlot(screenScale[0] * centerX),
                        FloatToStackSlot(screenScale[1] * centerY), 0);

    // Vertices: 32-byte entries { colour/uv @+0, pos.xy @+8, extra @+0x10 }.
    int vertexCount;
    if (self->field_0050 != 0)
        vertexCount = static_cast<int>(self->field_0054 - self->field_0050) >> 5;
    else
        vertexCount = 0;

    if (vertexCount != 0)
    {
        std::uint32_t entryOffset = 0;
        int remaining = vertexCount;
        do
        {
            float sourcePoint[3];
            float transformedPoint[3];
            float screenPoint[4] = {};

            std::uint8_t* entry = reinterpret_cast<std::uint8_t*>(self->field_0050) + entryOffset;
            sourcePoint[0] = CSF_AT(float, entry, 0x8);
            sourcePoint[1] = CSF_AT(float, entry, 0xC);
            sourcePoint[2] = 0.0f;

            RwV3dTransformPoints(reinterpret_cast<int>(transformedPoint), reinterpret_cast<int>(sourcePoint), 1,
                                 reinterpret_cast<int>(holder.matrix));

            screenPoint[0] = transformedPoint[0];
            screenPoint[1] = transformedPoint[1];

            entry = reinterpret_cast<std::uint8_t*>(self->field_0050) + entryOffset;
            builder = reinterpret_cast<void*>(self->field_001c);
            ((void(__fastcall*)(void*, void*, float*, void*, void*))CSF_AT(void**, builder, 0)[0x34 / 4])(
                builder, nullptr, screenPoint, entry + 0x10, entry);

            entryOffset += 32;
            --remaining;
        } while (remaining != 0);
    }

    // Indices: three triangles per ring segment, then the closing segment.
    int closingBase = 0;
    if (self->field_0020 - 1 != 0)
    {
        int ringIndex = 3;
        std::uint32_t segment = 0;
        do
        {
            RadarBuilder_AddIndex(self, ringIndex - 2);
            RadarBuilder_AddIndex(self, 0);
            RadarBuilder_AddIndex(self, ringIndex);

            RadarBuilder_AddIndex(self, ringIndex - 1);
            RadarBuilder_AddIndex(self, ringIndex - 2);
            RadarBuilder_AddIndex(self, ringIndex);

            RadarBuilder_AddIndex(self, ringIndex);
            RadarBuilder_AddIndex(self, ringIndex + 1);
            RadarBuilder_AddIndex(self, ringIndex - 1);

            closingBase += 2;
            ringIndex += 2;
            ++segment;
        } while (segment < self->field_0020 - 1);
    }

    const int lastInner = closingBase + 1;
    RadarBuilder_AddIndex(self, lastInner);
    RadarBuilder_AddIndex(self, 0);
    RadarBuilder_AddIndex(self, 1);

    const int lastOuter = closingBase + 2;
    RadarBuilder_AddIndex(self, lastOuter);
    RadarBuilder_AddIndex(self, lastInner);
    RadarBuilder_AddIndex(self, 1);

    RadarBuilder_AddIndex(self, 1);
    RadarBuilder_AddIndex(self, 2);
    RadarBuilder_AddIndex(self, lastOuter);

    char result = static_cast<char>(holder.owned);
    if (holder.owned)
        result = static_cast<char>(RwMatrixDestroy(holder.matrix));
    return result;
}

HOOK(0x0043D4B0, C_00830e5c_RebuildRadarGeometry, "auto");
