#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetWideTextLength, 0x00695560, (std::uint16_t* text));

static int __stdcall DevConsole_ReadWideToken(
    std::uint16_t* text, std::uint16_t* output)
{
    int position = 0;
    int outputLength = 0;

    if (!text || !output)
        return position;

    output[0] = 0;

    if (text[0] == 32)
    {
        do
        {
            if (position >= GetWideTextLength(text))
                break;
            ++position;
        } while (text[position] == 32);
    }

    std::uint16_t* cursor = text + position;

    if (*cursor == 34 && position < GetWideTextLength(text))
    {
        ++position;
        cursor = text + position;

        if (*cursor != 34)
        {
            while (position < GetWideTextLength(text))
            {
                output[outputLength++] = *cursor;
                ++cursor;
                ++position;

                if (*cursor == 34)
                    break;
            }
        }

        output[outputLength] = 0;
        return position;
    }

    if (*cursor != 32)
    {
        do
        {
            if (position >= GetWideTextLength(text))
                break;

            output[outputLength++] = *cursor;
            ++cursor;
            ++position;
        } while (*cursor != 32);
    }

    output[outputLength] = 0;
    return position;
}

HOOK(0x004FA800, DevConsole_ReadWideToken, "auto");
