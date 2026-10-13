#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, sub_694CD0, 0x00694CD0, (void* self, void* edx, int a2));

// Walks back from the end of the buffer to the last CRLF, then pads the
// trailing segment with spaces so that its length reaches a2 + 20.
static char __fastcall S_00694cd0_PadLastLineToWidth(S_00694cd0* self, void* /*edx*/, int a2)
{
    uint8_t flagByte = CSF_AT(uint8_t, self, 8 * a2 + 0x11C);
    if (flagByte != 0)
        return static_cast<char>(flagByte);

    int lengthCursor = static_cast<int>(self->field_000c);
    int lineBreakDistance = 0;
    if (lengthCursor > 1)
    {
        while (reinterpret_cast<uint8_t*>(self->field_0000)[lengthCursor - 1] != 10 ||
               reinterpret_cast<uint8_t*>(self->field_0000)[lengthCursor - 2] != 13)
        {
            --lengthCursor;
            ++lineBreakDistance;
            if (lengthCursor <= 1)
                return static_cast<char>(lengthCursor);
        }

        int padCount = a2 - lineBreakDistance + 20;
        if (padCount > 0)
        {
            sub_694CD0(self, nullptr, padCount);

            uint32_t writeIndex = self->field_000c;
            if (writeIndex < static_cast<uint32_t>(writeIndex + padCount))
            {
                do
                {
                    reinterpret_cast<uint8_t*>(self->field_0000)[writeIndex] = 32;
                    ++writeIndex;
                } while (writeIndex < static_cast<uint32_t>(padCount) + self->field_000c);
            }
            self->field_000c += padCount;
            return static_cast<char>(writeIndex);
        }
    }
    return static_cast<char>(lengthCursor);
}

HOOK(0x00694D50, S_00694cd0_PadLastLineToWidth, "auto");
