#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall S_00696af0_FindLastSubstring(
    S_00696af0* self, void* /*edx*/,
    const uint16_t* pattern, uint32_t position, uint32_t count)
{
    const uint32_t length = CSF_AT(uint32_t, self, 0x14);
    if (count == 0)
        return static_cast<int>(position < length ? position : length);

    if (count > length)
        return -1;

    uint32_t start = length - count;
    if (position < start)
        start = position;

    const uint32_t capacity = self->field_0018;
    const auto* inlineStorage =
        reinterpret_cast<const uint16_t*>(&self->field_0004);
    const uint16_t* base = capacity < 8
        ? inlineStorage
        : reinterpret_cast<const uint16_t*>(self->field_0004);
    const uint16_t* candidate = base + start;
    const uint16_t firstCharacter = *pattern;

    for (;;)
    {
        if (*candidate == firstCharacter)
        {
            const uint16_t* text = candidate;
            const uint16_t* needle = pattern;
            uint32_t remaining = count;

            while (*text == *needle)
            {
                ++text;
                ++needle;
                if (--remaining == 0)
                    break;
            }

            if (remaining == 0)
            {
                base = capacity < 8
                    ? inlineStorage
                    : reinterpret_cast<const uint16_t*>(self->field_0004);
                const int32_t byteOffset = static_cast<int32_t>(
                    reinterpret_cast<uintptr_t>(candidate) -
                    reinterpret_cast<uintptr_t>(base));
                return byteOffset >> 1;
            }
        }

        base = capacity < 8
            ? inlineStorage
            : reinterpret_cast<const uint16_t*>(self->field_0004);
        if (candidate == base)
            return -1;

        --candidate;
    }
}

HOOK(0x00733A80, S_00696af0_FindLastSubstring, "auto");
