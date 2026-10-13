#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __cdecl GfxClipHelpers_NextSector(void* context)
{
    CSF_AT(uint8_t*, context, 0x33C) -= 0xC;

    while (CSF_AT(int32_t,
                 CSF_AT(uint8_t*, CSF_AT(uint8_t*, context, 0x33C), 0),
                 0) >= 0)
    {
        uint8_t* entry = CSF_AT(uint8_t*, context, 0x33C);
        uint8_t* node = CSF_AT(uint8_t*, entry, 0);
        int32_t axisOffset = CSF_AT(int32_t, node, 0);

        float direction = CSF_AT(float, context, axisOffset + 0xC);
        float origin = CSF_AT(float, context, axisOffset);
        float radius = CSF_AT(float, context, axisOffset + 0x24);
        float endPosition = direction * CSF_AT(float, entry, 8) + origin;
        float startPosition = direction * CSF_AT(float, entry, 4) + origin;
        float lowerBound = CSF_AT(float, node, 0x14) - radius;
        float upperBound = radius + CSF_AT(float, node, 0x10);

        if (direction > g_fZero)
        {
            if (lowerBound > endPosition)
            {
                CSF_AT(uint32_t, entry, 0) = CSF_AT(uint32_t, node, 8);
                continue;
            }
            if (upperBound < startPosition)
            {
                CSF_AT(uint32_t, entry, 0) = CSF_AT(uint32_t, node, 0xC);
                continue;
            }

            uint8_t* pushedEntry = entry + 0xC;
            CSF_AT(uint8_t*, context, 0x33C) = pushedEntry;
            float inverseDirection =
                CSF_AT(float, context, CSF_AT(int32_t, node, 0) + 0x18);
            CSF_AT(uint32_t, pushedEntry, 0) = CSF_AT(uint32_t, node, 8);

            uint8_t* current = CSF_AT(uint8_t*, context, 0x33C);
            CSF_AT(uint32_t, current, 4) = CSF_AT(uint32_t, current, -8);
            current = CSF_AT(uint8_t*, context, 0x33C);
            CSF_AT(uint32_t, current, 8) = CSF_AT(uint32_t, current, -4);

            if (upperBound < endPosition)
            {
                current = CSF_AT(uint8_t*, context, 0x33C);
                CSF_AT(float, current, 8) =
                    (upperBound - endPosition) * inverseDirection +
                    CSF_AT(float, current, 8);
            }

            current = CSF_AT(uint8_t*, context, 0x33C);
            CSF_AT(uint32_t, current, -0xC) = CSF_AT(uint32_t, node, 0xC);
            if (lowerBound > startPosition)
            {
                current = CSF_AT(uint8_t*, context, 0x33C);
                CSF_AT(float, current, -8) =
                    (lowerBound - startPosition) * inverseDirection +
                    CSF_AT(float, current, -8);
            }
        }
        else
        {
            if (upperBound < endPosition)
            {
                CSF_AT(uint32_t, entry, 0) = CSF_AT(uint32_t, node, 0xC);
                continue;
            }
            if (lowerBound > startPosition)
            {
                CSF_AT(uint32_t, entry, 0) = CSF_AT(uint32_t, node, 8);
                continue;
            }

            uint8_t* pushedEntry = entry + 0xC;
            CSF_AT(uint8_t*, context, 0x33C) = pushedEntry;
            float inverseDirection =
                CSF_AT(float, context, CSF_AT(int32_t, node, 0) + 0x18);
            CSF_AT(uint32_t, pushedEntry, 0) = CSF_AT(uint32_t, node, 0xC);

            uint8_t* current = CSF_AT(uint8_t*, context, 0x33C);
            CSF_AT(uint32_t, current, 4) = CSF_AT(uint32_t, current, -8);
            current = CSF_AT(uint8_t*, context, 0x33C);
            CSF_AT(uint32_t, current, 8) = CSF_AT(uint32_t, current, -4);

            if (lowerBound > endPosition)
            {
                current = CSF_AT(uint8_t*, context, 0x33C);
                CSF_AT(float, current, 8) =
                    (lowerBound - endPosition) * inverseDirection +
                    CSF_AT(float, current, 8);
            }

            current = CSF_AT(uint8_t*, context, 0x33C);
            CSF_AT(uint32_t, current, -0xC) = CSF_AT(uint32_t, node, 8);
            if (upperBound < startPosition)
            {
                current = CSF_AT(uint8_t*, context, 0x33C);
                CSF_AT(float, current, -8) =
                    (upperBound - startPosition) * inverseDirection +
                    CSF_AT(float, current, -8);
            }
        }
    }

    return static_cast<int>(reinterpret_cast<uintptr_t>(
        CSF_AT(uint8_t*, CSF_AT(uint8_t*, context, 0x33C), 0)));
}

HOOK(0x007C1E90, GfxClipHelpers_NextSector, "auto");
