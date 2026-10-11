#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Maps a request code (0..14) to a category 0..3, or -1 when the code is unknown.
// The category depends on the state byte at +0x1D8 of the object referenced by field_0008.
static int __fastcall C_0083eb70_MapRequestToCategory(C_0083eb70* self, void* /*edx*/, int requestCode)
{
    uint32_t stateValue = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x1D8);
    int result = -1;

    switch (requestCode) {
    case 0:
    case 1:
        if (stateValue != 2 && stateValue != 1)
            result = 0;
        break;
    case 2:
    case 3:
        if (stateValue != 2)
            result = 1;
        break;
    case 4:
    case 5:
    case 6:
        result = 2;
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        result = 3;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x004E6E00, C_0083eb70_MapRequestToCategory, "auto");
