#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __fastcall, S_004f21d0_ReserveRangeAndGetCursor, 0x004F21D0, (S_004f21d0* self, void* , int* a2, int a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, InsertWideCharacters, 0x006959F0, (void* self, void* edx, unsigned int a2, unsigned int a3, int16_t a4));

static char __fastcall S_004f21d0_AssignFromNarrowString(
    S_004f21d0* self, void* /*edx*/, const uint8_t* text)
{
    // Listed by the packet, although the instruction is a direct call.
    (void)CSF_VTBL(0x004F21D0);

    alignas(8) uint8_t cursorBytes[0x4] = {};
    int* cursorOutput = reinterpret_cast<int*>(cursorBytes);

    const uint32_t capacity = self->field_0018;
    const uint32_t inlineAddress =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&self->field_0004));

    const uint32_t endBase = capacity < 8
        ? inlineAddress
        : CSF_AT(uint32_t, self, 0x4);
    const uint32_t endAddress = endBase + self->field_0014 * 2u;
    const uint32_t beginAddress = capacity < 8
        ? inlineAddress
        : CSF_AT(uint32_t, self, 0x4);

    S_004f21d0_ReserveRangeAndGetCursor(
        self, nullptr, cursorOutput,
        static_cast<int32_t>(beginAddress), endAddress);

    uint8_t character = *text;
    while (character != 0)
    {
        const uint32_t currentCapacity = self->field_0018;
        const int16_t wideCharacter =
            static_cast<int16_t>(static_cast<int8_t>(character));

        const uint32_t currentEndBase = currentCapacity < 8
            ? inlineAddress
            : CSF_AT(uint32_t, self, 0x4);
        uint32_t position = currentEndBase + self->field_0014 * 2u;
        const uint32_t currentBegin = currentCapacity < 8
            ? inlineAddress
            : CSF_AT(uint32_t, self, 0x4);

        if (position != 0)
            position = static_cast<uint32_t>(
                static_cast<int32_t>(position - currentBegin) >> 1);

        InsertWideCharacters(self, nullptr, position, 1u, wideCharacter);
        character = text[1];
        ++text;
    }
    return static_cast<char>(character);
}

HOOK(0x00697070, S_004f21d0_AssignFromNarrowString, "auto");
