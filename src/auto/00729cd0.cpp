#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00865a10_ApplyTextureTransform, 0x007295F0, (C_00865a10* self, void* , float angleDegrees, const uint32_t* translation, uint32_t scaleX, uint32_t scaleY));
CSF_FUNC(char, __fastcall, C_00865a10_ApplyEdgeTextureTransform, 0x00729710, (void* self, void* edx, float a2, float* a3, float* a4));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static char __fastcall C_00865a10_UpdateBorderTextureTransforms(
    C_00865a10* self, void* /*edx*/)
{
    alignas(8) uint8_t extentBytes[0x8]{};
    alignas(8) uint8_t translationBytes[0x10]{};
    alignas(8) uint8_t textureSizeBytes[0x8]{};
    alignas(8) uint8_t edgeEndBytes[0x8]{};
    alignas(8) uint8_t edgeStartBytes[0x10]{};

    float* extent = reinterpret_cast<float*>(extentBytes);
    float* translation = reinterpret_cast<float*>(translationBytes);
    float* textureSize = reinterpret_cast<float*>(textureSizeBytes);
    float* edgeEnd = reinterpret_cast<float*>(edgeEndBytes);
    float* edgeStart = reinterpret_cast<float*>(edgeStartBytes);
    float& width = edgeStart[2];
    float& height = edgeStart[3];

    void* referencedObject = self->vGetReferencedObject();
    uint32_t left;
    uint32_t top;
    uint32_t right;
    uint32_t bottom;

    if (self->field_0030 || self->field_0034 ||
        self->field_0038 || self->field_003c || !referencedObject)
    {
        left = self->field_0030;
        top = self->field_0034;
        right = self->field_0038;
        bottom = self->field_003c;
    }
    else
    {
        uint32_t* position =
            ((uint32_t* (__fastcall*)(void*, void*, float*))
                CSF_AT(void**, referencedObject, 0)[0x160 / 4])(
                    referencedObject, nullptr, translation);
        left = position[0];
        top = position[1];

        CSF_AT(uint32_t, translationBytes, 0x8) = left;
        CSF_AT(uint32_t, translationBytes, 0xC) = top;

        uint32_t* size =
            ((uint32_t* (__fastcall*)(void*, void*))
                CSF_AT(void**, referencedObject, 0)[0xA4 / 4])(
                    referencedObject, nullptr);
        right = left + size[0];
        bottom = top + size[1];
    }

    CSF_AT(uint32_t, translationBytes, 0x8) = left;
    CSF_AT(uint32_t, translationBytes, 0xC) = top;

    const float rectangleWidth = static_cast<float>(
        static_cast<int32_t>(right - left));
    const float rectangleHeight = static_cast<float>(
        static_cast<int32_t>(bottom - top));

    void* texture = reinterpret_cast<void*>(self->field_0050);
    const float horizontalMargin =
        static_cast<float>(static_cast<int32_t>(self->field_0048));
    edgeEnd[0] = static_cast<float>(static_cast<int32_t>(left)) -
                 horizontalMargin;
    const float verticalMargin =
        static_cast<float>(static_cast<int32_t>(self->field_004c));
    edgeEnd[1] = static_cast<float>(static_cast<int32_t>(top)) -
                 verticalMargin;

    extent[1] = edgeEnd[1];
    width = horizontalMargin + horizontalMargin + rectangleWidth;
    height = verticalMargin + verticalMargin + rectangleHeight;
    extent[0] = width + edgeEnd[0];

    ((void (__fastcall*)(void*, void*, float*))
        CSF_AT(void**, texture, 0)[0x24 / 4])(texture, nullptr, extent);

    translation[0] = 0.0f;
    translation[1] = 0.0f;
    C_00865a10_ApplyTextureTransform(
        self, nullptr, 0.0f,
        reinterpret_cast<const uint32_t*>(translation),
        1065353216, 1065353216);

    translation[0] = 0.0f;
    translation[1] = height;
    C_00865a10_ApplyTextureTransform(
        self, nullptr, 0.0f,
        reinterpret_cast<const uint32_t*>(translation),
        1065353216, static_cast<uint32_t>(-1082130432));

    edgeStart[1] = -width;
    translation[0] = edgeStart[1];
    translation[1] = height;
    C_00865a10_ApplyTextureTransform(
        self, nullptr, 180.0f,
        reinterpret_cast<const uint32_t*>(translation),
        1065353216, 1065353216);

    translation[0] = edgeStart[1];
    translation[1] = 0.0f;
    C_00865a10_ApplyTextureTransform(
        self, nullptr, 0.0f,
        reinterpret_cast<const uint32_t*>(translation),
        static_cast<uint32_t>(-1082130432), 1065353216);

    texture = reinterpret_cast<void*>(self->field_0050);
    float* dimensions =
        ((float* (__fastcall*)(void*, void*))
            CSF_AT(void**, texture, 0)[0x40 / 4])(texture, nullptr);
    const float textureWidth = dimensions[0];
    const float textureHeight = dimensions[1];
    const float originX = edgeEnd[0];
    const float originY = edgeEnd[1];

    textureSize[0] = textureWidth;
    const float halfTextureWidth = textureSize[0] * g_HalfDurationScale;
    textureSize[1] = textureHeight * g_HalfDurationScale;

    edgeStart[0] = originX + halfTextureWidth;
    edgeStart[1] = originY;
    edgeEnd[0] = width - halfTextureWidth + originX;
    edgeEnd[1] = originY;
    C_00865a10_ApplyEdgeTextureTransform(
        self, nullptr, 0.0f, edgeStart, edgeEnd);

    translation[0] = edgeStart[0];
    translation[1] = originY + height;
    edgeStart[0] = edgeEnd[0];
    edgeStart[1] = originY + height;
    C_00865a10_ApplyEdgeTextureTransform(
        self, nullptr, 180.0f, translation, edgeStart);

    edgeStart[1] = originY + textureSize[1];
    edgeStart[0] = originX;
    edgeEnd[0] = originX;
    edgeEnd[1] = height - textureSize[1] + originY;
    C_00865a10_ApplyEdgeTextureTransform(
        self, nullptr, -90.0f, edgeStart, edgeEnd);

    textureSize[1] = edgeStart[1];
    textureSize[0] = originX + width;
    translation[0] = textureSize[0];
    translation[1] = edgeEnd[1];
    return C_00865a10_ApplyEdgeTextureTransform(
        self, nullptr, 90.0f, textureSize, translation);
}

HOOK(0x00729CD0, C_00865a10_UpdateBorderTextureTransforms, "auto");
