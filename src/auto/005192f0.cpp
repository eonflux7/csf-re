#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Handles a small command code for C_00840ca8. Does nothing unless field_000c is
// nonzero. Codes 0/1 step a counter at +0x10 down/up (clamped to 0 when it goes
// negative), codes 2/3 set the flag at +0x25, code 13 clears field_000c.
// Returns 0 when field_000c is already zero, otherwise returns the command code.
// The third stack argument is never read by the original.
static int __fastcall C_00840ca8_HandleCommand(C_00840ca8* self, void* /*edx*/, int commandCode, int /*unused*/)
{
    int result = (int)self->field_000c;
    if (result)
    {
        result = commandCode;
        switch (commandCode)
        {
        case 0:
            self->field_0010 = self->field_0010 - 1;
            if ((int32_t)self->field_0010 < 0)
                self->field_0010 = 0;
            break;
        case 1:
            self->field_0010 = self->field_0010 + 1;
            if ((int32_t)self->field_0010 < 0)
                self->field_0010 = 0;
            break;
        case 2:
        case 3:
            self->field_0025 = 1;
            break;
        case 13:
            self->field_000c = 0;
            break;
        default:
            return result;
        }
    }
    return result;
}

HOOK(0x5192F0, C_00840ca8_HandleCommand, "auto");
