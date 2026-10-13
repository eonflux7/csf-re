#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, HudStringShow_UpdateScaledDistance, 0x004F1D00, (int objectAddress));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall S_004f1f00_UpdateSubtitleEntries(
    S_004f1f00* self, void* /*edx*/, float deltaTime)
{
    int count = 0;
    if (self->field_0008)
        count = static_cast<int32_t>(self->field_000c - self->field_0008) / 124;

    int result = count - 1;
    if (result < 0)
        return result;

    uint32_t offset = 124 * static_cast<uint32_t>(result);
    int remaining = result + 1;

    do
    {
        auto* entry = reinterpret_cast<uint8_t*>(self->field_0008) + offset;
        const uint32_t currentDistance = CSF_AT(uint32_t, entry, 0x60);
        const uint32_t startingDistance = CSF_AT(uint32_t, entry, 0x58);

        if (startingDistance == currentDistance &&
            currentDistance != CSF_AT(uint32_t, entry, 0x68))
        {
            HudStringShow_UpdateScaledDistance(
                static_cast<int>(reinterpret_cast<uintptr_t>(entry)));
            CSF_AT(float, entry, 0x44) = 0.0f;
        }

        if (CSF_AT(float, entry, 0x48) != g_fZero)
        {
            const float elapsed = deltaTime + CSF_AT(float, entry, 0x44);
            const bool stillInterpolating = elapsed < CSF_AT(float, entry, 0x48);
            CSF_AT(float, entry, 0x44) = elapsed;

            uint32_t distance;
            if (stillInterpolating)
            {
                const uint32_t start = CSF_AT(uint32_t, entry, 0x58);
                const int32_t difference = static_cast<int32_t>(
                    CSF_AT(uint32_t, entry, 0x68) - start);
                const int64_t adjustment = static_cast<int64_t>(
                    static_cast<float>(difference) /
                    CSF_AT(float, entry, 0x48) * elapsed);
                distance = CSF_AT(uint32_t, entry, 0x58) +
                           static_cast<uint32_t>(adjustment);
            }
            else
            {
                const uint32_t targetPosition = CSF_AT(uint32_t, entry, 0x64);
                distance = CSF_AT(uint32_t, entry, 0x68);
                CSF_AT(float, entry, 0x48) = 0.0f;
                CSF_AT(float, entry, 0x44) = 0.0f;
                CSF_AT(uint32_t, entry, 0x5C) = targetPosition;
                CSF_AT(uint32_t, entry, 0x54) = targetPosition;
                CSF_AT(uint32_t, entry, 0x58) = distance;
            }
            CSF_AT(uint32_t, entry, 0x60) = distance;
        }

        auto* updatedEntry = reinterpret_cast<uint8_t*>(self->field_0008) + offset;
        CSF_AT(float, updatedEntry, 0x3C) =
            deltaTime + CSF_AT(float, updatedEntry, 0x3C);
        offset -= 124;
        result = --remaining;
    }
    while (remaining);

    return result;
}

HOOK(0x004F1F00, S_004f1f00_UpdateSubtitleEntries, "auto");
