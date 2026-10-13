#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall S_004f1f00_FinalizeSubtitlePanel(
    S_004f1f00* self, void* /*edx*/)
{
    const uint32_t begin = self->field_0008;
    if (!begin)
        return 0;

    const uint32_t byteLength = self->field_000c - begin;
    const uint32_t count =
        static_cast<uint32_t>(static_cast<int32_t>(byteLength) / 124);

    if (!count)
        return static_cast<int32_t>(byteLength * 0x84210843u);

    uint32_t offset = 0;
    for (uint32_t index = 0; index < count; ++index, offset += 124)
    {
        auto* base = reinterpret_cast<uint8_t*>(self->field_0008);
        auto* current = base + offset;

        if (CSF_AT(uint8_t, current, 0x74))
            continue;

        uint32_t otherIndex = 0;
        auto* other = base;
        for (; otherIndex < count; ++otherIndex, other += 124)
        {
            if (otherIndex == index)
                continue;

            const int32_t otherStart = CSF_AT(int32_t, other, 0x60);
            const int32_t otherWidth = CSF_AT(int32_t, other, 0x50);
            const int32_t otherEnd = static_cast<int32_t>(
                static_cast<uint32_t>(otherWidth) +
                static_cast<uint32_t>(otherStart));

            const int32_t currentStart = CSF_AT(int32_t, current, 0x60);
            const int32_t currentWidth = CSF_AT(int32_t, current, 0x50);
            const int32_t currentEnd = static_cast<int32_t>(
                static_cast<uint32_t>(currentWidth) +
                static_cast<uint32_t>(currentStart));

            if (otherStart > currentStart && otherStart < currentEnd)
                break;
            if (otherEnd > currentStart && otherEnd < currentEnd)
                break;

            const int32_t reloadedOtherStart =
                CSF_AT(int32_t, other, 0x60);
            const int32_t reloadedOtherEnd = static_cast<int32_t>(
                static_cast<uint32_t>(otherWidth) +
                static_cast<uint32_t>(reloadedOtherStart));

            if (currentStart > reloadedOtherStart &&
                currentStart < reloadedOtherEnd)
                break;
            if (currentEnd > reloadedOtherStart &&
                currentEnd < reloadedOtherEnd)
                break;
        }

        CSF_AT(uint8_t, current, 0x74) =
            static_cast<uint8_t>(otherIndex >= count);
    }

    return static_cast<int32_t>(count);
}

HOOK(0x004F22A0, S_004f1f00_FinalizeSubtitlePanel, "auto");
