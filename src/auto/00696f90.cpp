#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int*, __fastcall, EraseWideStringRange, 0x004F21D0, (legacy::wstring* text, void* edx, int* result, std::uint32_t first, std::uint32_t last));
CSF_FUNC(void*, __fastcall, InsertWideCharacters, 0x006959F0, (void* self, void* edx, unsigned int a2, unsigned int a3, int16_t a4));

[[maybe_unused]] static auto const g_EraseWideStringRangeVtable =
    CSF_VTBL(0x004F21D0);

static std::uint32_t* __fastcall S_004f21d0_AssignText(
    S_004f21d0* self, void* /*edx*/, std::int16_t* source)
{
    alignas(8) std::uint8_t resultBytes[0x4];

    const std::uint32_t capacity = self->field_0018;
    std::uint16_t* endStorage = capacity < 8
        ? &self->field_0004
        : *reinterpret_cast<std::uint16_t**>(&self->field_0004);
    const std::uint32_t endAddress =
        reinterpret_cast<std::uint32_t>(endStorage) + self->field_0014 * 2;

    std::uint16_t* beginStorage = capacity < 8
        ? &self->field_0004
        : *reinterpret_cast<std::uint16_t**>(&self->field_0004);

    EraseWideStringRange(
        reinterpret_cast<legacy::wstring*>(self), nullptr,
        reinterpret_cast<int*>(resultBytes),
        reinterpret_cast<std::uint32_t>(beginStorage), endAddress);

    for (std::int16_t character = *source; character != 0; character = *source)
    {
        const std::uint32_t currentCapacity = self->field_0018;
        std::uint16_t* currentEndStorage = currentCapacity < 8
            ? &self->field_0004
            : *reinterpret_cast<std::uint16_t**>(&self->field_0004);
        std::uint32_t position =
            reinterpret_cast<std::uint32_t>(currentEndStorage)
            + self->field_0014 * 2;

        std::uint16_t* currentBeginStorage = currentCapacity < 8
            ? &self->field_0004
            : *reinterpret_cast<std::uint16_t**>(&self->field_0004);

        if (position != 0)
        {
            position = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    position - reinterpret_cast<std::uint32_t>(currentBeginStorage))
                >> 1);
        }

        InsertWideCharacters(self, nullptr, position, 1u, character);
        ++source;
    }

    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x00696F90, S_004f21d0_AssignText, "auto");
