#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int16_t __cdecl GfxClipHelpers_NextTriangleRange(
    uint32_t* context, uint16_t* firstTriangle, uint16_t* triangleCount)
{
    CSF_AT(uint8_t*, context, 0x1C8) -= 12;

    while (CSF_AT(uint8_t, CSF_AT(uint8_t*,
               CSF_AT(uint8_t*, context, 0x1C8), 0), 1) == 0xFF)
    {
        uint8_t* entry = CSF_AT(uint8_t*, context, 0x1C8);
        uint8_t* node = CSF_AT(uint8_t*,
            CSF_AT(uint8_t*, context, 0), 0x20)
            + 16 * CSF_AT(uint16_t, CSF_AT(uint8_t*, entry, 0), 2);

        const unsigned axisOffset = CSF_AT(uint8_t, node, 0) & 0xC;
        uint8_t* axis = reinterpret_cast<uint8_t*>(context) + axisOffset;
        const float direction = CSF_AT(float, axis, 16);
        const float origin = CSF_AT(float, axis, 4);
        const float extent = CSF_AT(float, axis, 40);
        const float endPosition = direction * CSF_AT(float, entry, 8) + origin;
        const float startPosition = direction * CSF_AT(float, entry, 4) + origin;
        const float lowerBound = CSF_AT(float, node, 12) - extent;
        const float upperBound = extent + CSF_AT(float, node, 4);

        if (!(direction > g_fZero))
        {
            if (!(upperBound < endPosition))
            {
                if (!(lowerBound > startPosition))
                {
                    CSF_AT(uint8_t*, context, 0x1C8) = entry + 12;
                    const float inverseDirection = CSF_AT(float, axis, 28);
                    CSF_AT(uint8_t*, entry, 12) = node + 8;

                    uint8_t* top = CSF_AT(uint8_t*, context, 0x1C8);
                    CSF_AT(uint32_t, top, 4) = CSF_AT(uint32_t, top, -8);
                    top = CSF_AT(uint8_t*, context, 0x1C8);
                    CSF_AT(uint32_t, top, 8) = CSF_AT(uint32_t, top, -4);

                    if (lowerBound > endPosition)
                    {
                        top = CSF_AT(uint8_t*, context, 0x1C8);
                        CSF_AT(float, top, 8) =
                            (lowerBound - endPosition) * inverseDirection
                            + CSF_AT(float, top, 8);
                    }

                    top = CSF_AT(uint8_t*, context, 0x1C8);
                    CSF_AT(uint8_t*, top, -12) = node;
                    if (!(upperBound < startPosition))
                        continue;

                    top = CSF_AT(uint8_t*, context, 0x1C8);
                    CSF_AT(float, top, -8) =
                        (upperBound - startPosition) * inverseDirection
                        + CSF_AT(float, top, -8);
                    continue;
                }
                CSF_AT(uint8_t*, entry, 0) = node;
            }
            else
            {
                CSF_AT(uint8_t*, entry, 0) = node + 8;
            }
        }
        else if (!(lowerBound > endPosition))
        {
            if (!(upperBound < startPosition))
            {
                CSF_AT(uint8_t*, context, 0x1C8) = entry + 12;
                const float inverseDirection = CSF_AT(float, axis, 28);
                CSF_AT(uint8_t*, entry, 12) = node;

                uint8_t* top = CSF_AT(uint8_t*, context, 0x1C8);
                CSF_AT(uint32_t, top, 4) = CSF_AT(uint32_t, top, -8);
                top = CSF_AT(uint8_t*, context, 0x1C8);
                CSF_AT(uint32_t, top, 8) = CSF_AT(uint32_t, top, -4);

                if (upperBound < endPosition)
                {
                    top = CSF_AT(uint8_t*, context, 0x1C8);
                    CSF_AT(float, top, 8) =
                        (upperBound - endPosition) * inverseDirection
                        + CSF_AT(float, top, 8);
                }

                top = CSF_AT(uint8_t*, context, 0x1C8);
                CSF_AT(uint8_t*, top, -12) = node + 8;
                if (!(lowerBound > startPosition))
                    continue;

                top = CSF_AT(uint8_t*, context, 0x1C8);
                CSF_AT(float, top, -8) =
                    (lowerBound - startPosition) * inverseDirection
                    + CSF_AT(float, top, -8);
                continue;
            }
            CSF_AT(uint8_t*, entry, 0) = node + 8;
        }
        else
        {
            CSF_AT(uint8_t*, entry, 0) = node;
        }
    }

    *firstTriangle = CSF_AT(uint16_t,
        CSF_AT(uint8_t*, CSF_AT(uint8_t*, context, 0x1C8), 0), 2);
    const int16_t count = CSF_AT(uint8_t,
        CSF_AT(uint8_t*, CSF_AT(uint8_t*, context, 0x1C8), 0), 1);
    *triangleCount = static_cast<uint16_t>(count);
    return count;
}

HOOK(0x007C2640, GfxClipHelpers_NextTriangleRange, "auto");
