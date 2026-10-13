#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall Geometry_AppendColoredBox(
    void* self, void* /*edx*/, float* minimum, float* maximum,
    std::uint8_t* color)
{
    const int firstVertex = CSF_AT(int, self, 0x0C);
    const int indices[24] = {
        0, 1, 1, 3, 3, 2,
        2, 0, 4, 5, 5, 7,
        7, 6, 6, 4, 0, 4,
        1, 5, 2, 6, 3, 7
    };

    std::uint8_t* finalVertexBuffer = nullptr;

    for (int corner = 0; corner < 8; ++corner)
    {
        std::uint8_t* vertexBuffer = corner == 7
            ? finalVertexBuffer
            : CSF_AT(std::uint8_t*, self, 0x04);
        const int vertexIndex = CSF_AT(int, self, 0x0C);

        const float z = (corner & 4) ? maximum[2] : minimum[2];
        const float y = (corner & 2) ? maximum[1] : minimum[1];
        const float x = (corner & 1) ? maximum[0] : minimum[0];

        CSF_AT(float, vertexBuffer + 36 * vertexIndex, 0x00) = x;
        CSF_AT(float,
               CSF_AT(std::uint8_t*, self, 0x04)
                   + 36 * CSF_AT(int, self, 0x0C),
               0x04) = y;
        CSF_AT(float,
               CSF_AT(std::uint8_t*, self, 0x04)
                   + 36 * CSF_AT(int, self, 0x0C),
               0x08) = z;

        const std::uint32_t alpha = color[3];
        const std::uint32_t red = color[0];
        const std::uint32_t green = color[1];
        const std::uint32_t blue = color[2];
        CSF_AT(std::uint32_t,
               CSF_AT(std::uint8_t*, self, 0x04)
                   + 36 * CSF_AT(int, self, 0x0C),
               0x18) =
            blue | (green << 8) | (red << 16) | (alpha << 24);

        if (corner < 5)
        {
            const int nextVertex = CSF_AT(int, self, 0x0C) + 1;
            CSF_AT(std::uint8_t, self, 0x2C) = 0;
            CSF_AT(int, self, 0x0C) = nextVertex;
        }
        else
        {
            if (corner == 6)
                finalVertexBuffer = CSF_AT(std::uint8_t*, self, 0x04);

            ++CSF_AT(int, self, 0x0C);
            CSF_AT(std::uint8_t, self, 0x2C) = 0;
        }
    }

    const std::uint16_t indexBase = static_cast<std::uint16_t>(firstVertex);
    int result;
    for (result = 0; result < 24; result += 6)
    {
        CSF_AT(std::uint16_t,
               CSF_AT(std::uint8_t*, self, 0x10)
                   + 2 * CSF_AT(int, self, 0x18),
               0) = static_cast<std::uint16_t>(indexBase + indices[result]);

        for (int offset = 1; offset < 6; ++offset)
        {
            std::uint8_t* indexBuffer = CSF_AT(std::uint8_t*, self, 0x10);
            const std::uint16_t index =
                static_cast<std::uint16_t>(indexBase + indices[result + offset]);
            const int nextIndex = CSF_AT(int, self, 0x18) + 1;
            CSF_AT(int, self, 0x18) = nextIndex;
            CSF_AT(std::uint16_t, indexBuffer + 2 * nextIndex, 0) = index;
        }

        ++CSF_AT(int, self, 0x18);
    }

    CSF_AT(int, self, 0x1C) = 1;
    return result;
}

HOOK(0x006C3A20, Geometry_AppendColoredBox, "auto");
