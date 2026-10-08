#include <cstdint>
#include "hook.h"

static int __stdcall MenuMpAccounts_HasInvalidAccountNameCharacters(
    const char* name, int allowAnyCharacter)
{
    bool valid = true;

    if (name)
    {
        const char* cursor = name;
        char character;
        do
        {
            character = *cursor++;
        } while (character != '\0');

        const int32_t length = static_cast<int32_t>(
            reinterpret_cast<uint32_t>(cursor) -
            reinterpret_cast<uint32_t>(name) - 1u);

        for (int32_t index = 0; index < length; ++index)
        {
            if (!valid)
                break;

            if (!allowAnyCharacter)
            {
                const int8_t value = static_cast<int8_t>(name[index]);
                valid = (value >= 65 && value <= 90) ||
                        (value >= 97 && value <= 122) ||
                        (value >= 48 && value <= 57);
            }
        }
    }

    return valid ? 0 : 1;
}

HOOK(0x0055C240, MenuMpAccounts_HasInvalidAccountNameCharacters, "auto");
