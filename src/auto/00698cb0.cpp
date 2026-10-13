#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __stdcall, ValidateWordValue, 0x00698C50, (uint16_t* a1));

// Reads the next 16-bit value from the cursor in S_00698c10 (field_0018, limit field_001c).
// The cursor advances only when the two bytes fit before the limit. The value is validated
// by ValidateWordValue unless the read ran past the limit. *okFlag gets 1 on success, 0 otherwise.
static int16_t __fastcall ReadWordFromCursor(S_00698c10* self, void* /*edx*/, uint8_t* okFlag)
{
    uint32_t cursor = self->field_0018;
    uint32_t nextCursor = cursor + 2;
    bool readPastEnd = false;
    uint16_t word;

    if (nextCursor <= self->field_001c)
    {
        word = *reinterpret_cast<const uint16_t*>(cursor);
        self->field_0018 = nextCursor;
    }
    else
    {
        word = *reinterpret_cast<const uint16_t*>(cursor);
        readPastEnd = true;
    }

    uint32_t value = word;
    if (readPastEnd || !ValidateWordValue(reinterpret_cast<uint16_t*>(&value)))
    {
        *okFlag = 0;
        return static_cast<int16_t>(value);
    }

    *okFlag = 1;
    return static_cast<int16_t>(value);
}

HOOK(0x698CB0, ReadWordFromCursor, "auto");
