#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int*, __fastcall, S_004f21d0_ReserveRangeAndGetCursor, 0x004F21D0,
         (S_004f21d0* self, void* , int* cursor, int begin, unsigned int end));
CSF_FUNC(void*, __fastcall, InsertWideCharacters, 0x006959F0,
         (void* self, void* edx, unsigned int position, unsigned int count, int character));

[[maybe_unused]] static auto const g_AssignTextVtable = CSF_VTBL(0x004F21D0);

static std::uint32_t* __fastcall S_004f21d0_AssignText(
    S_004f21d0* self, void* /*edx*/, std::int16_t* source)
{
    alignas(4) std::uint8_t cursorBytes[0x4];
    int* cursor = reinterpret_cast<int*>(cursorBytes);

    const std::uint32_t capacity = self->field_0018;
    std::uint16_t* endStorage = capacity < 8
        ? &self->field_0004
        : *reinterpret_cast<std::uint16_t**>(&self->field_0004);
    const std::uint32_t endAddress =
        reinterpret_cast<std::uint32_t>(endStorage) + self->field_0014 * 2;

    std::uint16_t* beginStorage = capacity < 8
        ? &self->field_0004
        : *reinterpret_cast<std::uint16_t**>(&self->field_0004);

    S_004f21d0_ReserveRangeAndGetCursor(
        self, nullptr, cursor,
        static_cast<int>(reinterpret_cast<std::uint32_t>(beginStorage)), endAddress);

    // 00696fcb: `mov cx, word ptr [edi]` without zeroing ecx first, so the first
    // `push ecx` (00696ffd) carries the high 16 bits ecx still holds after the
    // ReserveRangeAndGetCursor call (= self). Later iterations do `xor ecx,ecx`.
    std::uint16_t character = *source;
    bool firstIteration = true;
    for (; character != 0; )
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

        unsigned int characterArg;
        if (firstIteration)
        {
            characterArg = (static_cast<std::uint32_t>(
                                reinterpret_cast<std::uintptr_t>(self))
                            & 0xFFFF0000u)
                         | static_cast<std::uint16_t>(character);
            firstIteration = false;
        }
        else
        {
            characterArg = static_cast<std::uint16_t>(character);
        }

        InsertWideCharacters(self, nullptr, position, 1u, static_cast<int>(characterArg));
        ++source;
        character = *source;
    }

    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x00696F90, S_004f21d0_AssignText, "auto");
