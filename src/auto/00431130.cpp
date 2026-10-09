#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, S_0042eb60_DrawDigit, 0x00431080,
         (S_0042eb60* self, void* edx, unsigned int digit, int style,
          uint32_t* position, char alternate));

static void __fastcall S_0042eb60_DrawNumber(
    S_0042eb60* self, void* /*edx*/, unsigned int number,
    int style, int alignment, char alternate)
{
    if (!self->field_011b)
        return;

    auto* definition = reinterpret_cast<uint8_t*>(self->field_0034)
                     + 20u * static_cast<uint32_t>(style);

    const uint32_t left = CSF_AT(uint32_t, definition, 0x0);
    const uint32_t top = CSF_AT(uint32_t, definition, 0x4);
    const uint32_t right = CSF_AT(uint32_t, definition, 0x8);
    const uint32_t bottom = CSF_AT(uint32_t, definition, 0xC);
    (void)top;
    (void)bottom;

    uint32_t digitCount = 0;
    uint32_t remaining = number;
    do
    {
        remaining /= 0xAu;
        ++digitCount;
    } while (remaining);

    uint32_t minimumDigits = 0;
    uint32_t templateNumber = CSF_AT(uint32_t, definition, 0x10);
    const int32_t digitWidth = static_cast<int32_t>(right - left);
    while (templateNumber)
    {
        templateNumber /= 0xAu;
        ++minimumDigits;
    }

    uint32_t origin;
    switch (alignment)
    {
    case 0:
        origin = left + (digitCount - 1) *
                        static_cast<uint32_t>(digitWidth / 2);
        break;
    case 1:
        origin = left;
        break;
    case 2:
        origin = left + static_cast<uint32_t>(digitWidth) *
                        (digitCount - 1);
        break;
    default:
        return;
    }

    uint32_t position[2] = {origin, 0};
    uint32_t drawnDigits = 0;
    uint32_t digitIndex = 0;
    remaining = number;

    do
    {
        ++drawnDigits;
        const uint32_t digit = remaining % 0xA;
        position[1] = digitIndex++;
        remaining /= 0xAu;
        S_0042eb60_DrawDigit(self, nullptr, digit, style, position, alternate);
    } while (remaining);

    if (drawnDigits < minimumDigits)
    {
        position[0] = origin;
        uint32_t padding = minimumDigits - drawnDigits;
        do
        {
            position[1] = digitIndex++;
            S_0042eb60_DrawDigit(self, nullptr, 0, style, position, alternate);
        } while (--padding);
    }
}

HOOK(0x00431130, S_0042eb60_DrawNumber, "auto");
