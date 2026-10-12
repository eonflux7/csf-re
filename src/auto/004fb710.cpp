#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall S_004fb710_ExtractArgument(
    S_004fb710* self, void* /*edx*/, unsigned int argumentIndex,
    char* output)
{
    const char* cursor = self->field_0018 < 0x10
        ? reinterpret_cast<const char*>(&self->field_0004)
        : reinterpret_cast<const char*>(self->field_0004);

    unsigned int currentArgument = 0;
    int outputLength = 0;
    bool inQuotes = false;
    char result = 0;

    char character = *cursor;
    while (character)
    {
        if (currentArgument > argumentIndex)
            break;

        if (character == 32 && !inQuotes)
        {
            ++currentArgument;
        }
        else if (character == 34)
        {
            inQuotes = !inQuotes;
        }
        else if (currentArgument == argumentIndex)
        {
            output[outputLength++] = character;
            result = 1;
        }

        character = *++cursor;
    }

    output[outputLength] = 0;
    return result;
}

HOOK(0x004FB710, S_004fb710_ExtractArgument, "auto");
