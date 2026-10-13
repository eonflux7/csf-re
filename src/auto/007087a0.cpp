#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GfxObjectProps_ResetPropertyBuffer, 0x006B8CC0, (int a1));

static int __cdecl GfxAnimInterpolator_UpdateTrianglePropertyAverages(int a1)
{
    void* object = reinterpret_cast<void*>(a1);
    auto* properties = reinterpret_cast<std::uint8_t*>(
        GfxObjectProps_ResetPropertyBuffer(a1));

    int triangleIndex = 0;
    int triangleOffset = 0;

    if (CSF_AT(std::uint16_t, object, 0x84) > 0)
    {
        do
        {
            auto* triangles = CSF_AT(std::uint8_t*, object, 0x04);
            const std::uint16_t firstIndex =
                CSF_AT(std::uint16_t, triangles + triangleOffset, 0);

            auto* colors = CSF_AT(std::uint8_t*, object, 0x30);
            auto* firstColor = colors + 4 * firstIndex;
            const unsigned int firstSum =
                CSF_AT(std::uint8_t, firstColor, 1) +
                CSF_AT(std::uint8_t, firstColor, 2) +
                CSF_AT(std::uint8_t, firstColor, 0);

            const std::uint16_t secondIndex =
                CSF_AT(std::uint16_t, triangles + triangleOffset, 2);
            auto* secondColor = colors + 4 * secondIndex;
            const unsigned int secondBlue =
                CSF_AT(std::uint8_t, secondColor, 2);

            const std::uint16_t thirdIndex =
                CSF_AT(std::uint16_t, triangles + triangleOffset, 4);
            const unsigned int secondSum =
                secondBlue +
                CSF_AT(std::uint8_t, secondColor, 1) +
                CSF_AT(std::uint8_t, secondColor, 0);

            auto* thirdColor = colors + 4 * thirdIndex;
            const unsigned int thirdBlue =
                CSF_AT(std::uint8_t, thirdColor, 2);
            const unsigned int thirdGreen =
                CSF_AT(std::uint8_t, thirdColor, 1);
            const unsigned int thirdRed =
                CSF_AT(std::uint8_t, thirdColor, 0);

            properties[triangleIndex] = static_cast<std::uint8_t>(
                (firstSum + secondSum + thirdRed + thirdGreen + thirdBlue) / 9);

            const int triangleCount = CSF_AT(std::uint16_t, object, 0x84);
            ++triangleIndex;
            triangleOffset += 8;
            if (triangleIndex >= triangleCount)
                break;
        }
        while (true);
    }

    return a1;
}

HOOK(0x007087A0, GfxAnimInterpolator_UpdateTrianglePropertyAverages, "auto");
