#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int16_t, __cdecl, CoreVariantValues_ReadWideCharacter, 0x00695660, (void** a1));
CSF_FUNC(char, __cdecl, CoreVariantValues_ParsePlaceholderValue, 0x00695CA0, (void** a1, uint32_t* arg4));

static char __cdecl CoreVariantValues_ParseWidePlaceholder(
    int16_t** cursor,
    int16_t*** result,
    int16_t* character,
    bool* atEnd)
{
    int16_t* position = *cursor;
    const int16_t first = *position;

    if (first == 0)
    {
        *atEnd = true;
        *character = 0;
        return 0;
    }

    ++position;
    *cursor = position;

    if (first != 123)
    {
        *atEnd = false;
        *character = first;
        return 0;
    }

    const uint16_t type = static_cast<uint16_t>(*position);
    if (type != 0)
    {
        ++position;
        *cursor = position;
    }

    if (type == 0)
    {
        *character = 123;
        return 0;
    }

    if (type != 100 && type != 115 && type != 102 && type != 119)
    {
        *cursor = position - 1;
        *character = 123;
        return 0;
    }

    const int16_t next = *position;
    if (next != 0)
    {
        ++position;
        *cursor = position;
    }
    *cursor = position - 1;

    if (next == 0)
    {
        *character = 123;
        return 0;
    }

    uint32_t value = 0;
    if (!CoreVariantValues_ParsePlaceholderValue(
            reinterpret_cast<void**>(cursor), &value))
    {
        *cursor -= 1;
        *character = 123;
        return 0;
    }

    const int16_t closing = CoreVariantValues_ReadWideCharacter(
        reinterpret_cast<void**>(cursor));

    if (closing == 125)
    {
        CSF_AT(uint32_t, result, 0) = value;
        CSF_AT(uint32_t, result, 0x4) = type;
        return 1;
    }

    if (closing != 0)
        *cursor -= 3;
    else
        *cursor -= 2;

    *character = 123;
    return 0;
}

HOOK(0x00695DE0, CoreVariantValues_ParseWidePlaceholder, "auto");
