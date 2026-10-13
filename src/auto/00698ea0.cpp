#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00698E40: __thiscall, no stack args, returns char (tested with `cmp al, bl`).
CSF_FUNC(char, __fastcall, TableCursor_IsSkipPending, 0x00698E40, (void* self, void* edx));

// 0x00698BD0: __thiscall, one stack arg (ret 4), returns a 16-bit character.
CSF_FUNC(int16_t, __fastcall, TableCursor_ReadEscapedChar, 0x00698BD0, (void* self, void* edx, uint8_t* a2));

// Consumes a '/' separator token from the table text cursor. Returns 1 when a '/'
// is read and the following token is also '/', otherwise rewinds the cursor by 2.
static char __fastcall TableCursor_TryConsumeSlashPair(S_00698c10* self, void* /*edx*/)
{
    if (!TableCursor_IsSkipPending(self, nullptr)) {
        uint32_t cursor = self->field_0018;
        uint32_t end = self->field_001c;
        uint32_t next = cursor + 2;
        uint8_t hadError = 0;

        if (next <= end) {
            int16_t firstChar = *reinterpret_cast<int16_t*>(cursor);
            self->field_0018 = next;
            if (firstChar == 0x2F) {
                int16_t secondChar = TableCursor_ReadEscapedChar(self, nullptr, &hadError);
                if (hadError == 0 && secondChar == 0x2F) {
                    return 1;
                }
            }
        }
        self->field_0018 -= 2;
    }
    return 0;
}

HOOK(0x00698EA0, TableCursor_TryConsumeSlashPair, "auto");
