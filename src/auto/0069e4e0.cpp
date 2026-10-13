#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static uint8_t __fastcall S_0069e480_ClassifyCurrentToken(
    S_0069e480* self, void* /*edx*/)
{
    const uint32_t position = self->field_0018;
    const uint8_t* current =
        reinterpret_cast<const uint8_t*>(self->field_0008 + position);
    uint8_t character = *current;

    if (character == 34)
    {
        self->field_0020 = 1;
        return character;
    }

    if (character <= 0x20u)
    {
        self->field_0020 = 0;
        return character;
    }

    if ((character >= 0x30u && character <= 0x39u) ||
        (character >= 0x61u && character <= 0x7Au) ||
        (character >= 0x41u && character <= 0x5Au) ||
        character == 95 || character == 46 || character == 45)
    {
        self->field_0020 = 3;
        return character;
    }

    uint32_t tokenType = 6;
    if (position == self->field_000c - 1 || character != 47)
    {
        tokenType = 2;
    }
    else
    {
        character = current[1];
        if (character == 42)
            tokenType = 5;

        if (character == 47)
            tokenType = 4;
        else if (tokenType == 6)
            tokenType = 2;
    }

    self->field_0020 = tokenType;
    return character;
}

HOOK(0x0069E4E0, S_0069e480_ClassifyCurrentToken, "auto");
