#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(double, __fastcall, CalculateColorChannelIntensity, 0x004387E0, (void* self, void* , int position));

static __declspec(noinline) int32_t ReadRectangleCoordinate(
    const void* data, std::size_t offset)
{
    int32_t value;
    std::memcpy(&value, static_cast<const uint8_t*>(data) + offset, sizeof(value));
    return value;
}

static int32_t AddRectangleCoordinates(int32_t first, int32_t second)
{
    return static_cast<int32_t>(
        static_cast<uint32_t>(first) + static_cast<uint32_t>(second));
}

static int32_t SubtractRectangleCoordinates(int32_t first, int32_t second)
{
    return static_cast<int32_t>(
        static_cast<uint32_t>(first) - static_cast<uint32_t>(second));
}

static void __fastcall DrawHorizontallyPartitionedRectangle(
    void* self, void* /*edx*/, void* renderer,
    float* textureCoordinates, int32_t* position, int32_t* size)
{
    if (!renderer)
        return;

    // Keep the scratch members in the same objects passed to the renderer.
    // In particular, the interpolated coordinates belong to the UV object:
    // a submission is allowed to modify them.
    alignas(8) uint8_t frameBytes[0x38];
    float* color = reinterpret_cast<float*>(frameBytes);
    float* vertexPosition = reinterpret_cast<float*>(frameBytes + 0x10);
    float* vertexTexture = reinterpret_cast<float*>(frameBytes + 0x18);

    const int32_t lowerBoundary = CSF_AT(int32_t, self, 0xDC);
    const int32_t initialStart = ReadRectangleCoordinate(position, 0x00);
    color[0] = 255.0f;
    color[1] = 255.0f;
    color[2] = 255.0f;
    CSF_AT(int32_t, vertexTexture, 0x1C) = initialStart;

    // Edge identifiers: start, lower boundary, upper boundary, end.
    int edges[4] = {0, 3, 0, 0};
    int partitionCount = 1;
    enum PartitionMode { Unpartitioned, LowerOnly, UpperOnly, Both };
    PartitionMode mode = Unpartitioned;
    float lowerOnlyTextureCoordinate = 0.0f;

    if (initialStart < lowerBoundary)
    {
        const int32_t width = ReadRectangleCoordinate(size, 0x00);
        const int32_t upperBoundary = CSF_AT(int32_t, self, 0xE4);
        CSF_AT(int32_t, vertexTexture, 0x0C) = width;

        if (AddRectangleCoordinates(initialStart, width) > upperBoundary)
        {
            const double textureSpan = static_cast<double>(textureCoordinates[2]) - textureCoordinates[0];
            CSF_AT(float, vertexTexture, 0x14) =
                static_cast<double>(SubtractRectangleCoordinates(
                    lowerBoundary, initialStart)) *
                textureSpan / static_cast<double>(width) + textureCoordinates[0];
            CSF_AT(float, vertexTexture, 0x0C) =
                static_cast<double>(SubtractRectangleCoordinates(
                    upperBoundary, initialStart)) *
                textureSpan / static_cast<double>(width) + textureCoordinates[0];

            edges[1] = 1;
            edges[2] = 2;
            edges[3] = 3;
            partitionCount = 3;
            mode = Both;
        }
        else
        {
            const int32_t remainingWidth = ReadRectangleCoordinate(size, 0x00);
            CSF_AT(int32_t, vertexTexture, 0x0C) = remainingWidth;
            const int32_t end = AddRectangleCoordinates(initialStart, remainingWidth);
            if (end >= lowerBoundary &&
                end <= CSF_AT(int32_t, self, 0xE4))
            {
                lowerOnlyTextureCoordinate =
                    static_cast<double>(SubtractRectangleCoordinates(
                        lowerBoundary, initialStart)) *
                    (static_cast<double>(textureCoordinates[2]) - textureCoordinates[0]) /
                    static_cast<double>(remainingWidth) + textureCoordinates[0];
                edges[1] = 1;
                edges[2] = 3;
                partitionCount = 2;
                mode = LowerOnly;
            }
        }
    }
    else
    {
        const int32_t upperBoundary = CSF_AT(int32_t, self, 0xE4);
        if (initialStart <= upperBoundary)
        {
            const int32_t width = ReadRectangleCoordinate(size, 0x00);
            CSF_AT(int32_t, vertexTexture, 0x0C) = width;
            if (AddRectangleCoordinates(initialStart, width) > upperBoundary)
            {
                CSF_AT(float, vertexTexture, 0x0C) =
                    static_cast<double>(SubtractRectangleCoordinates(
                        upperBoundary, initialStart)) *
                    (static_cast<double>(textureCoordinates[2]) - textureCoordinates[0]) /
                    static_cast<double>(width) + textureCoordinates[0];
                edges[1] = 2;
                edges[2] = 3;
                partitionCount = 2;
                mode = UpperOnly;
            }
        }
    }

    for (int partition = 0; partition < partitionCount; ++partition)
    {
        for (int vertex = 0; vertex < 6; ++vertex)
        {
            const bool right = vertex == 2 || vertex == 3 || vertex == 4;
            const bool bottom = vertex == 1 || vertex == 2 || vertex == 3;
            const int edge = edges[partition + (right ? 1 : 0)];

            int32_t x;
            switch (edge)
            {
            case 0:
                x = partition == 0 && vertex == 0
                    ? initialStart : ReadRectangleCoordinate(position, 0x00);
                break;
            case 1:
                x = CSF_AT(int32_t, self, 0xDC);
                break;
            case 2:
                x = CSF_AT(int32_t, self, 0xE4);
                break;
            default:
                x = AddRectangleCoordinates(
                    ReadRectangleCoordinate(size, 0x00),
                    ReadRectangleCoordinate(position, 0x00));
                break;
            }

            int32_t y = ReadRectangleCoordinate(position, 0x04);
            if (bottom)
                y = AddRectangleCoordinates(
                    y, ReadRectangleCoordinate(size, 0x04));

            std::size_t scratchOffset = 0x08;
            if (mode == Both)
            {
                scratchOffset =
                    partition == 0 || (partition == 1 && vertex < 5)
                    ? 0x18 : 0x10;
            }
            else if (mode == UpperOnly)
            {
                scratchOffset = 0x10;
            }
            if (partition == partitionCount - 1 && vertex == 5)
                scratchOffset = 0x08;

            if (edge == 0 && !(partition == 0 && vertex == 0))
                CSF_AT(int32_t, vertexTexture, 0x1C) = x;

            // The first vertex converts in the UV pair itself. At the two
            // shared-tail transitions, position and UV use different scratch pairs.
            std::size_t positionScratchOffset = scratchOffset;
            if (partition == 0 && vertex == 0)
                positionScratchOffset = 0;
            else if (mode == Both && partition == 1 && vertex == 5)
                positionScratchOffset = 0x18;
            else if ((mode == Both || mode == UpperOnly) &&
                     partition == partitionCount - 1 && vertex == 5)
                positionScratchOffset = 0x10;

            CSF_AT(float, vertexTexture, positionScratchOffset) = static_cast<float>(x);
            CSF_AT(float, vertexTexture, positionScratchOffset + 4) = static_cast<float>(y);
            vertexPosition[0] = CSF_AT(float, vertexTexture, positionScratchOffset);
            vertexPosition[1] = CSF_AT(float, vertexTexture, positionScratchOffset + 4);

            float u;
            switch (edge)
            {
            case 0:
                u = textureCoordinates[0];
                break;
            case 1:
                u = mode == LowerOnly
                    ? lowerOnlyTextureCoordinate
                    : CSF_AT(float, vertexTexture, 0x14);
                break;
            case 2:
                u = CSF_AT(float, vertexTexture, 0x0C);
                break;
            default:
                u = textureCoordinates[2];
                break;
            }
            const float v = textureCoordinates[bottom ? 3 : 1];

            CSF_AT(float, vertexTexture, scratchOffset) = u;
            CSF_AT(float, vertexTexture, scratchOffset + 4) = v;
            vertexTexture[0] = u;
            vertexTexture[1] = v;

            color[3] = static_cast<float>(
                CalculateColorChannelIntensity(self, nullptr, x));
            ((void(__fastcall*)(void*, void*, float*, float*, float*))
                CSF_AT(void**, renderer, 0)[0x34 / 4])(
                    renderer, nullptr, vertexPosition, color, vertexTexture);
        }
    }
}

HOOK(0x00438880, DrawHorizontallyPartitionedRectangle, "auto");
