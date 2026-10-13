#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, HudStringShow_UpdateScaledDistance, 0x004F1D00, (int objectAddress));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall S_004f1f00_UpdateSubtitleLayout(
    S_004f1f00* self, void* /*edx*/)
{
    int result = static_cast<int32_t>(self->field_0008);
    auto* first = reinterpret_cast<uint8_t*>(self->field_0008);
    int32_t groupX = CSF_AT(int32_t, first, 0x6C);
    int32_t groupY = CSF_AT(int32_t, first, 0x70);

    uint32_t count = 0;
    if (self->field_0008 != 0) {
        result = static_cast<int32_t>(
            self->field_000c - self->field_0008) / 124;
        count = static_cast<uint32_t>(result);
    }

    uint32_t byteOffset = 0;
    uint32_t accumulatedSpacing = 0;
    for (uint32_t i = 0; i < count; ++i, byteOffset += 124) {
        auto* entry =
            reinterpret_cast<uint8_t*>(self->field_0008) + byteOffset;

        if (CSF_AT(int32_t, entry, 0x6C) != groupX ||
            CSF_AT(int32_t, entry, 0x70) != groupY) {
            accumulatedSpacing = 0;
            groupX = CSF_AT(int32_t, entry, 0x6C);
            groupY = CSF_AT(int32_t, entry, 0x70);
        }

        const int32_t oldY = CSF_AT(int32_t, entry, 0x68);
        CSF_AT(int32_t, entry, 0x68) = groupY;

        const uint32_t adjustment = accumulatedSpacing
            ? accumulatedSpacing * self->field_0034
            : 0;
        entry = reinterpret_cast<uint8_t*>(self->field_0008) + byteOffset;
        CSF_AT(uint32_t, entry, 0x68) += adjustment;

        entry = reinterpret_cast<uint8_t*>(self->field_0008) + byteOffset;
        if (CSF_AT(float, entry, 0x48) > g_fZero &&
            oldY != CSF_AT(int32_t, entry, 0x68)) {
            const int32_t startY = CSF_AT(int32_t, entry, 0x58);
            const int32_t endY = CSF_AT(int32_t, entry, 0x60);
            const int32_t newY = CSF_AT(int32_t, entry, 0x68);

            if (!(startY >= endY && endY >= newY) &&
                (startY > endY || endY > newY)) {
                CSF_AT(uint32_t, entry, 0x54) =
                    CSF_AT(uint32_t, entry, 0x5C);
                CSF_AT(uint32_t, entry, 0x58) =
                    CSF_AT(uint32_t, entry, 0x60);

                entry = reinterpret_cast<uint8_t*>(self->field_0008) + byteOffset;
                CSF_AT(uint32_t, entry, 0x44) = 0;
            }

            HudStringShow_UpdateScaledDistance(
                static_cast<int32_t>(self->field_0008 + byteOffset));
        }

        const uint32_t direction = self->field_0034;
        if (direction == 1) {
            entry = reinterpret_cast<uint8_t*>(self->field_0008) + byteOffset;
            accumulatedSpacing +=
                self->field_0030 + CSF_AT(uint32_t, entry, 0x50);
        }

        if (direction == static_cast<uint32_t>(-1) && i + 1 < count) {
            entry = reinterpret_cast<uint8_t*>(self->field_0008) + byteOffset;
            accumulatedSpacing +=
                self->field_0030 + CSF_AT(uint32_t, entry, 0xCC);
        }

        result = static_cast<int32_t>(i + 1);
    }

    return result;
}

HOOK(0x004F1FF0, S_004f1f00_UpdateSubtitleLayout, "auto");
