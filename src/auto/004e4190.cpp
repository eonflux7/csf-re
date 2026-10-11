// Reimplementation of 0x004E4190 (C_0083e7d8 method, __thiscall, ret 12).
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

// Returns true when the command code (a2) should be treated as flagged for this object.
// Codes 6/18 flag when the linked state object's status (+0x50) is not 4.
// Codes 7/8/19 flag unless the status is 4 and a3 is 4.
// Code 12 is always flagged. Any other code is flagged only for (-1, 4).
static bool __fastcall C_0083e7d8_EvaluateCommandCodeFlag(C_0083e7d8* self, void* /*edx*/,
                                                          int commandCode, int argument3, int /*argument4*/)
{
    bool result = false;

    switch (commandCode)
    {
    case 6:
    case 18:
    {
        void* stateObject = reinterpret_cast<void*>(self->field_000c);
        result = CSF_AT(int, stateObject, 0x50) != 4;
        break;
    }
    case 7:
    case 8:
    case 19:
    {
        void* stateObject = reinterpret_cast<void*>(self->field_000c);
        if (CSF_AT(int, stateObject, 0x50) != 4 || argument3 != 4)
            result = true;
        else
            result = false;
        break;
    }
    case 12:
        result = true;
        break;
    default:
        if (commandCode == -1 && argument3 == 4)
            result = true;
        break;
    }

    return result;
}

HOOK(0x4E4190, C_0083e7d8_EvaluateCommandCodeFlag, "auto");
