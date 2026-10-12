#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(double, __fastcall, GetHorizontalVertexValue, 0x004387E0,
         (void* self, void* edx, int a2));

static int32_t AddCoordinates(int32_t a, int32_t b)
{
    return static_cast<int32_t>(static_cast<uint32_t>(a) +
                                static_cast<uint32_t>(b));
}

static int32_t SubtractCoordinates(int32_t a, int32_t b)
{
    return static_cast<int32_t>(static_cast<uint32_t>(a) -
                                static_cast<uint32_t>(b));
}

static int32_t RectangleRight(const void* position, const void* size)
{
    return AddCoordinates(CSF_AT(int32_t, size, 0),
                          CSF_AT(int32_t, position, 0));
}

static int32_t RectangleBottom(const void* position, const void* size)
{
    return AddCoordinates(CSF_AT(int32_t, size, 4),
                          CSF_AT(int32_t, position, 4));
}

static void EmitTexturedVertex(void* self, void* renderer,
                              uint8_t* colorBytes, uint8_t* positionBytes,
                              uint8_t* textureBytes,
                              int32_t x, int32_t y, float u, float v)
{
    CSF_AT(float, positionBytes, 0) = static_cast<float>(x);
    CSF_AT(float, positionBytes, 4) = static_cast<float>(y);
    CSF_AT(float, textureBytes, 0) = u;
    CSF_AT(float, textureBytes, 4) = v;
    CSF_AT(float, colorBytes, 0xC) =
        static_cast<float>(GetHorizontalVertexValue(self, nullptr, x));

    ((void(__fastcall*)(void*, void*, float*, float*, float*))
         CSF_AT(void**, renderer, 0)[0x34 / 4])(
        renderer, nullptr,
        reinterpret_cast<float*>(positionBytes),
        reinterpret_cast<float*>(colorBytes),
        reinterpret_cast<float*>(textureBytes));
}

static void __fastcall DrawHorizontallyPartitionedRectangle(
    void* self, void* /*edx*/, void* renderer, float* texture,
    const void* position, const void* size)
{
    if (!renderer)
        return;

    alignas(8) uint8_t colorBytes[0x10] = {};
    alignas(8) uint8_t positionBytes[0x8] = {};
    alignas(8) uint8_t textureBytes[0x20] = {};

    CSF_AT(uint32_t, colorBytes, 0) = 1132396544;
    CSF_AT(uint32_t, colorBytes, 4) = 1132396544;
    CSF_AT(uint32_t, colorBytes, 8) = 1132396544;

    const int32_t leftBoundary = CSF_AT(int32_t, self, 0xDC);
    const int32_t initialX = CSF_AT(int32_t, position, 0);
    bool splitLeft = false;
    bool splitRight = false;
    float leftOnlyU = 0.0f;

    // These two interpolants occupy the original texture buffer's trailing
    // storage and survive the intervening vertex submissions.
    float& leftU = CSF_AT(float, textureBytes, 0x14);
    float& rightU = CSF_AT(float, textureBytes, 0xC);

    if (initialX < leftBoundary)
    {
        const int32_t width = CSF_AT(int32_t, size, 0);
        const int32_t rightBoundary = CSF_AT(int32_t, self, 0xE4);
        if (AddCoordinates(initialX, width) > rightBoundary)
        {
            const float textureWidth = texture[2] - texture[0];
            leftU = static_cast<float>(
                        SubtractCoordinates(leftBoundary, initialX)) *
                        textureWidth / static_cast<float>(width) + texture[0];
            rightU = static_cast<float>(
                         SubtractCoordinates(rightBoundary, initialX)) *
                         textureWidth / static_cast<float>(width) + texture[0];
            splitLeft = true;
            splitRight = true;
        }
        else
        {
            const int32_t leftWidth = CSF_AT(int32_t, size, 0);
            const int32_t endX = AddCoordinates(initialX, leftWidth);
            if (endX >= leftBoundary &&
                endX <= CSF_AT(int32_t, self, 0xE4))
            {
                leftOnlyU = static_cast<float>(
                                SubtractCoordinates(leftBoundary, initialX)) *
                                (texture[2] - texture[0]) /
                                static_cast<float>(leftWidth) + texture[0];
                splitLeft = true;
            }
        }
    }
    else
    {
        const int32_t rightBoundary = CSF_AT(int32_t, self, 0xE4);
        if (initialX <= rightBoundary)
        {
            const int32_t width = CSF_AT(int32_t, size, 0);
            if (AddCoordinates(initialX, width) > rightBoundary)
            {
                rightU = static_cast<float>(
                             SubtractCoordinates(rightBoundary, initialX)) *
                             (texture[2] - texture[0]) /
                             static_cast<float>(width) + texture[0];
                splitRight = true;
            }
        }
    }

    EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                       initialX, CSF_AT(int32_t, position, 4),
                       texture[0], texture[1]);
    EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                       CSF_AT(int32_t, position, 0),
                       RectangleBottom(position, size), texture[0], texture[3]);

    if (splitLeft)
    {
        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, self, 0xDC),
                           RectangleBottom(position, size),
                           splitRight ? leftU : leftOnlyU, texture[3]);
        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, self, 0xDC),
                           RectangleBottom(position, size),
                           splitRight ? leftU : leftOnlyU, texture[3]);
        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, self, 0xDC),
                           CSF_AT(int32_t, position, 4),
                           splitRight ? leftU : leftOnlyU, texture[1]);
        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, position, 0),
                           CSF_AT(int32_t, position, 4),
                           texture[0], texture[1]);

        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, self, 0xDC),
                           CSF_AT(int32_t, position, 4),
                           splitRight ? leftU : leftOnlyU, texture[1]);
        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, self, 0xDC),
                           RectangleBottom(position, size),
                           splitRight ? leftU : leftOnlyU, texture[3]);

        if (splitRight)
        {
            EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                               CSF_AT(int32_t, self, 0xE4),
                               RectangleBottom(position, size), rightU, texture[3]);
            EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                               CSF_AT(int32_t, self, 0xE4),
                               RectangleBottom(position, size), rightU, texture[3]);
            EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                               CSF_AT(int32_t, self, 0xE4),
                               CSF_AT(int32_t, position, 4), rightU, texture[1]);
            EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                               CSF_AT(int32_t, self, 0xDC),
                               CSF_AT(int32_t, position, 4), leftU, texture[1]);

            EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                               CSF_AT(int32_t, self, 0xE4),
                               CSF_AT(int32_t, position, 4), rightU, texture[1]);
            EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                               CSF_AT(int32_t, self, 0xE4),
                               RectangleBottom(position, size), rightU, texture[3]);
        }
    }
    else if (splitRight)
    {
        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, self, 0xE4),
                           RectangleBottom(position, size), rightU, texture[3]);
        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, self, 0xE4),
                           RectangleBottom(position, size), rightU, texture[3]);
        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, self, 0xE4),
                           CSF_AT(int32_t, position, 4), rightU, texture[1]);
        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, position, 0),
                           CSF_AT(int32_t, position, 4), texture[0], texture[1]);

        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, self, 0xE4),
                           CSF_AT(int32_t, position, 4), rightU, texture[1]);
        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, self, 0xE4),
                           RectangleBottom(position, size), rightU, texture[3]);
    }

    EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                       RectangleRight(position, size),
                       RectangleBottom(position, size), texture[2], texture[3]);
    EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                       RectangleRight(position, size),
                       RectangleBottom(position, size), texture[2], texture[3]);
    EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                       RectangleRight(position, size),
                       CSF_AT(int32_t, position, 4), texture[2], texture[1]);

    if (splitRight)
    {
        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, self, 0xE4),
                           CSF_AT(int32_t, position, 4), rightU, texture[1]);
    }
    else if (splitLeft)
    {
        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, self, 0xDC),
                           CSF_AT(int32_t, position, 4), leftOnlyU, texture[1]);
    }
    else
    {
        EmitTexturedVertex(self, renderer, colorBytes, positionBytes, textureBytes,
                           CSF_AT(int32_t, position, 0),
                           CSF_AT(int32_t, position, 4), texture[0], texture[1]);
    }
}

HOOK(0x00438880, DrawHorizontallyPartitionedRectangle, "auto");
