#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620, (std::uint16_t* source, char* destination, unsigned int length));
CSF_FUNC(int, __fastcall, sub_732C20, 0x00732C20, (void* self, void* edx));

static char __fastcall C_008670f0_ResetEntriesAndRefreshText(
    C_008670f0* self, void* /*edx*/)
{
    std::uint32_t result = static_cast<std::uint32_t>(sub_732C20(self, nullptr));
    if (static_cast<std::uint8_t>(result) == 0)
        return static_cast<char>(result);

    alignas(8) std::uint8_t textBytes[0x400];
    alignas(8) std::uint8_t secondOutputBytes[0x8];
    alignas(8) std::uint8_t firstOutputBytes[0x8];

    const std::uint32_t entryCount = self->field_0080;
    result = 0;

    if (static_cast<std::int32_t>(entryCount) >= 4)
    {
        std::uint32_t groups = ((entryCount - 4) >> 2) + 1;
        const std::uint32_t processedCount = 4 * groups;
        std::uint32_t offset = 0;

        do
        {
            auto* entry = reinterpret_cast<std::uint8_t*>(self->field_0078) + offset;
            CSF_AT(float, entry, 0x10) = 0.0f;
            CSF_AT(float, entry, 0x14) = 0.0f;
            CSF_AT(float, entry, 0x0C) = 255.0f;

            entry = reinterpret_cast<std::uint8_t*>(self->field_0078) + offset + 28;
            CSF_AT(float, entry, 0x0C) = 255.0f;
            CSF_AT(float, entry, 0x10) = 0.0f;
            CSF_AT(float, entry, 0x14) = 0.0f;

            entry = reinterpret_cast<std::uint8_t*>(self->field_0078) + offset + 84 - 28;
            CSF_AT(float, entry, 0x0C) = 255.0f;
            CSF_AT(float, entry, 0x10) = 0.0f;
            CSF_AT(float, entry, 0x14) = 0.0f;

            entry = reinterpret_cast<std::uint8_t*>(self->field_0078) + offset + 84;
            offset += 112;
            --groups;
            CSF_AT(float, entry, 0x0C) = 255.0f;
            CSF_AT(float, entry, 0x10) = 0.0f;
            CSF_AT(float, entry, 0x14) = 0.0f;
        } while (groups != 0);

        result = processedCount;
    }

    if (result < entryCount)
    {
        std::uint32_t offset = 28 * result;
        std::uint32_t remaining = entryCount - result;
        do
        {
            result = self->field_0078 + offset;
            auto* entry = reinterpret_cast<std::uint8_t*>(result);
            offset += 28;
            --remaining;
            CSF_AT(float, entry, 0x0C) = 255.0f;
            CSF_AT(float, entry, 0x10) = 0.0f;
            CSF_AT(float, entry, 0x14) = 0.0f;
        } while (remaining != 0);
    }

    if (self->field_00d8 != 0)
    {
        auto* source = self->field_0038 < 8
            ? reinterpret_cast<std::uint16_t*>(&self->field_0024)
            : reinterpret_cast<std::uint16_t*>(self->field_0024);
        ConvertWideTextToNarrow(source, reinterpret_cast<char*>(textBytes), 1024);

        result = self->field_00c8;
        if (result == 0)
        {
            self->vSlot5C(reinterpret_cast<std::uint32_t*>(firstOutputBytes));
            result = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(
                self->vSlot5C(reinterpret_cast<std::uint32_t*>(secondOutputBytes))));
        }
    }

    return static_cast<char>(result);
}

HOOK(0x00734360, C_008670f0_ResetEntriesAndRefreshText, "auto");
