#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, S_006abb80_WriteBoxCorners, 0x006ABC40, (S_006abb80* self, void* , float* corners));

static void GfxTeam_WriteBoundsVertex(
    void* self, uint8_t* vertex, const float* position,
    const uint8_t* color, bool advanceCount)
{
    CSF_AT(float, vertex, 0) = position[0];
    CSF_AT(float,
        CSF_AT(uint8_t*, self, 0x04) + 36 * CSF_AT(uint32_t, self, 0x0C),
        0x04) = position[1];
    CSF_AT(float,
        CSF_AT(uint8_t*, self, 0x04) + 36 * CSF_AT(uint32_t, self, 0x0C),
        0x08) = position[2];

    const uint32_t alpha = color[3];
    const uint32_t red = color[0];
    const uint32_t green = color[1];
    const uint32_t blue = color[2];
    const uint32_t packedColor =
        blue | (green << 8) | (red << 16) | (alpha << 24);

    uint8_t* buffer = CSF_AT(uint8_t*, self, 0x04);
    const uint32_t index = CSF_AT(uint32_t, self, 0x0C);
    if (advanceCount)
        ++CSF_AT(uint32_t, self, 0x0C);
    CSF_AT(uint32_t, buffer + 36 * index, 0x18) = packedColor;
}

static int __fastcall GfxTeam_DrawBounds(
    void* self, void* /*edx*/, S_006abb80* bounds, uint8_t* color)
{
    static constexpr uint32_t edgeCorners[24] = {
        0, 1, 1, 2, 2, 3,
        3, 0, 4, 5, 5, 6,
        6, 7, 7, 4, 0, 4,
        1, 5, 2, 6, 3, 7
    };

    alignas(8) uint8_t cornerBytes[0x60] = {};
    float* corners = reinterpret_cast<float*>(cornerBytes);
    S_006abb80_WriteBoxCorners(bounds, nullptr, corners);

    const uint32_t firstVertex = CSF_AT(uint32_t, self, 0x0C);
    GfxTeam_WriteBoundsVertex(
        self, CSF_AT(uint8_t*, self, 0x04) + 36 * firstVertex,
        corners, color, false);

    uint32_t vertexIndex = CSF_AT(uint32_t, self, 0x0C);
    uint8_t* vertexBuffer = CSF_AT(uint8_t*, self, 0x04);
    CSF_AT(uint8_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x0C) = ++vertexIndex;
    GfxTeam_WriteBoundsVertex(
        self, vertexBuffer + 36 * vertexIndex, corners + 3, color, false);

    vertexBuffer = CSF_AT(uint8_t*, self, 0x04);
    vertexIndex = CSF_AT(uint32_t, self, 0x0C) + 1;
    CSF_AT(uint32_t, self, 0x0C) = vertexIndex;
    CSF_AT(uint8_t, self, 0x2C) = 0;
    GfxTeam_WriteBoundsVertex(
        self, vertexBuffer + 36 * vertexIndex, corners + 6, color, false);

    vertexIndex = CSF_AT(uint32_t, self, 0x0C) + 1;
    CSF_AT(uint8_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x0C) = vertexIndex;
    GfxTeam_WriteBoundsVertex(
        self, CSF_AT(uint8_t*, self, 0x04) + 36 * vertexIndex,
        corners + 9, color, false);

    vertexIndex = CSF_AT(uint32_t, self, 0x0C);
    vertexBuffer = CSF_AT(uint8_t*, self, 0x04);
    CSF_AT(uint8_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x0C) = ++vertexIndex;
    GfxTeam_WriteBoundsVertex(
        self, vertexBuffer + 36 * vertexIndex, corners + 12, color, false);

    vertexBuffer = CSF_AT(uint8_t*, self, 0x04);
    vertexIndex = CSF_AT(uint32_t, self, 0x0C) + 1;
    CSF_AT(uint32_t, self, 0x0C) = vertexIndex;
    CSF_AT(uint8_t, self, 0x2C) = 0;
    GfxTeam_WriteBoundsVertex(
        self, vertexBuffer + 36 * vertexIndex, corners + 15, color, false);

    vertexIndex = CSF_AT(uint32_t, self, 0x0C);
    vertexBuffer = CSF_AT(uint8_t*, self, 0x04);
    CSF_AT(uint8_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x0C) = ++vertexIndex;
    GfxTeam_WriteBoundsVertex(
        self, vertexBuffer + 36 * vertexIndex, corners + 18, color, false);

    vertexBuffer = CSF_AT(uint8_t*, self, 0x04);
    vertexIndex = CSF_AT(uint32_t, self, 0x0C) + 1;
    CSF_AT(uint32_t, self, 0x0C) = vertexIndex;
    CSF_AT(uint8_t, self, 0x2C) = 0;
    GfxTeam_WriteBoundsVertex(
        self, vertexBuffer + 36 * vertexIndex, corners + 21, color, true);
    CSF_AT(uint8_t, self, 0x2C) = 0;

    int edgeOffset;
    for (edgeOffset = 0; edgeOffset < 24; edgeOffset += 6)
    {
        CSF_AT(uint16_t,
            CSF_AT(uint8_t*, self, 0x10) +
                2 * CSF_AT(uint32_t, self, 0x18),
            0) = static_cast<uint16_t>(firstVertex + edgeCorners[edgeOffset]);

        for (int endpoint = 1; endpoint < 6; ++endpoint)
        {
            uint8_t* indexBuffer = CSF_AT(uint8_t*, self, 0x10);
            const uint16_t index = static_cast<uint16_t>(
                firstVertex + edgeCorners[edgeOffset + endpoint]);
            const uint32_t indexPosition = CSF_AT(uint32_t, self, 0x18) + 1;
            CSF_AT(uint32_t, self, 0x18) = indexPosition;
            CSF_AT(uint16_t, indexBuffer + 2 * indexPosition, 0) = index;
        }
        ++CSF_AT(uint32_t, self, 0x18);
    }

    CSF_AT(uint32_t, self, 0x1C) = 1;
    return edgeOffset;
}

HOOK(0x006C3E60, GfxTeam_DrawBounds, "auto");
