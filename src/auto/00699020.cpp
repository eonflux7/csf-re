#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee at 0x698C10: called when a "//" comment is found; returns a 16-bit value that the caller ignores.
CSF_FUNC(int16_t, __fastcall, ScanToCrLfPairInBuffer, 0x00698C10, (void* self, void* edx));

// Skips blanks, tabs, CRLF line breaks and "//" comments in a UTF-16 text buffer.
// field_0018 is the read cursor, field_001c is the buffer end (exclusive).
// Returns 1 when the end of the buffer is reached, 0 otherwise (cursor left on the unread character).
static char __fastcall CoreSortedTable_SkipBlanksAndComments(S_00698c10* self, void* /*edx*/)
{
    for (;;)
    {
        uint32_t cursor = 0;
        uint32_t next = 0;
        uint32_t end = 0;
        int16_t ch = 0;

        // Skip spaces and tabs.
        do
        {
            cursor = self->field_0018;
            end = self->field_001c;
            next = cursor + 2;
            if (next > end)
                return 1;
            ch = *reinterpret_cast<int16_t*>(static_cast<uintptr_t>(cursor));
            self->field_0018 = next;
        } while (ch == 32 || ch == 9);

        if (ch == 13)
        {
            // CR: must be followed by LF to count as a line break.
            uint32_t afterCr = next + 2;
            if (afterCr > end)
            {
                self->field_0018 -= 2;
                return 0;
            }
            int16_t afterCrChar = *reinterpret_cast<int16_t*>(static_cast<uintptr_t>(next));
            self->field_0018 = afterCr;
            if (afterCrChar == 10)
                continue;

            self->field_0018 = next;
            self->field_0018 = cursor;
            return 0;
        }

        if (ch != 47) // not '/'
        {
            self->field_0018 -= 2;
            return 0;
        }

        // '/': check for a second '/' that starts a line comment.
        uint32_t afterSlash = next + 2;
        if (afterSlash > end)
        {
            self->field_0018 -= 2;
            return 0;
        }
        int16_t secondChar = *reinterpret_cast<int16_t*>(static_cast<uintptr_t>(self->field_0018));
        self->field_0018 = afterSlash;
        if (secondChar != 47)
        {
            self->field_0018 = next;
            self->field_0018 -= 2;
            return 0;
        }

        ScanToCrLfPairInBuffer(self, nullptr);
    }
}

// 0x699C80 does `mov ecx, esi; call 0x699020; ... call 0x699B10` with no reload of ecx.
HOOK_KEEP_ECX(0x00699020, CoreSortedTable_SkipBlanksAndComments, 0, "auto");

