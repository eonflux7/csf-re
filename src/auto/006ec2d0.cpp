#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall C_0086200c_GetIntersectionResult(
    C_0086200c* self, void* /*edx*/, void* output)
{
    if (self->field_0018)
        return static_cast<char>(self->field_0018);

    const uint32_t index = CSF_AT(uint32_t, self, 0x14);
    const uint32_t queryType = CSF_AT(uint32_t, self, 0x34);
    auto* record = reinterpret_cast<uint8_t*>(self->field_0004 + 104 * index);

    if (queryType == 1)
    {
        const float startX = CSF_AT(float, self, 0x1C);
        const float startY = CSF_AT(float, self, 0x20);
        const float startZ = CSF_AT(float, self, 0x24);
        const float deltaX = CSF_AT(float, self, 0x28) - startX;
        const float deltaY = CSF_AT(float, self, 0x2C) - startY;
        const float deltaZ = CSF_AT(float, self, 0x30) - startZ;

        CSF_AT(float, output, 0x00) =
            std::sqrt(deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX) *
            CSF_AT(float, record, 0x00);

        CSF_AT(uint32_t, output, 0x10) = CSF_AT(uint32_t, record, 0x10);
        CSF_AT(uint32_t, output, 0x14) = CSF_AT(uint32_t, record, 0x14);
        CSF_AT(uint32_t, output, 0x18) = CSF_AT(uint32_t, record, 0x18);
        CSF_AT(uint32_t, output, 0x1C) = CSF_AT(uint32_t, record, 0x1C);
        CSF_AT(uint8_t, output, 0x20) = CSF_AT(uint8_t, record, 0x20);

        const float fraction = CSF_AT(float, record, 0x00);
        CSF_AT(float, output, 0x04) = deltaX * fraction + startX;
        CSF_AT(float, output, 0x08) = deltaY * fraction + startY;
        CSF_AT(float, output, 0x0C) = deltaZ * fraction + startZ;
        CSF_AT(uint32_t, output, 0x24) = CSF_AT(uint32_t, record, 0x24);

        // Preserve the original forward dword copy, including overlap behavior.
        for (uint32_t offset = 0; offset < 0x40; offset += 4)
        {
            CSF_AT(uint32_t, output, 0x28 + offset) =
                CSF_AT(uint32_t, record, 0x28 + offset);
        }
    }
    else if (queryType == 3)
    {
        CSF_AT(float, output, 0x00) = std::sqrt(CSF_AT(float, record, 0x00));
        CSF_AT(uint32_t, output, 0x10) = CSF_AT(uint32_t, record, 0x10);
        CSF_AT(uint32_t, output, 0x14) = CSF_AT(uint32_t, record, 0x14);
        CSF_AT(uint32_t, output, 0x18) = CSF_AT(uint32_t, record, 0x18);
        CSF_AT(uint32_t, output, 0x1C) = CSF_AT(uint32_t, record, 0x1C);
        CSF_AT(uint8_t, output, 0x20) = CSF_AT(uint8_t, record, 0x20);
        CSF_AT(uint32_t, output, 0x04) = CSF_AT(uint32_t, record, 0x04);
        CSF_AT(uint32_t, output, 0x08) = CSF_AT(uint32_t, record, 0x08);
        CSF_AT(uint32_t, output, 0x0C) = CSF_AT(uint32_t, record, 0x0C);

        const uint32_t result = CSF_AT(uint32_t, record, 0x24);
        CSF_AT(uint32_t, output, 0x24) = result;
        return static_cast<char>(result);
    }

    return static_cast<char>(reinterpret_cast<uintptr_t>(record));
}

HOOK(0x006EC2D0, C_0086200c_GetIntersectionResult, "auto");
