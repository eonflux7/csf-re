#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Reads the flag byte (+0x42) of the object referenced by owner->field_0034,
// where owner is this->field_0008.
static bool C_008346e8_IsAltVariant(C_008346e8* self)
{
    C_008344f0* owner = reinterpret_cast<C_008344f0*>(self->field_0008);
    void* inner = reinterpret_cast<void*>(owner->field_0034);
    return CSF_AT(uint8_t, inner, 0x42) != 0;
}

static int __fastcall C_008346e8_MapIndex(C_008346e8* self, void* /*edx*/, int index)
{
    int result = -1;
    switch (index)
    {
    case 0:
        result = C_008346e8_IsAltVariant(self) ? 0xA : 0;
        break;
    case 1:
        result = (C_008346e8_IsAltVariant(self) ? 1 : 0) + 1;
        break;
    case 3:
        result = (C_008346e8_IsAltVariant(self) ? 1 : 0) + 3;
        break;
    case 5:
        result = (C_008346e8_IsAltVariant(self) ? 1 : 0) + 7;
        break;
    case 7:
        result = (C_008346e8_IsAltVariant(self) ? 1 : 0) + 5;
        break;
    case 9:
        result = 9;
        break;
    case 10:
        result = 10;
        break;
    case 11:
        result = (C_008346e8_IsAltVariant(self) ? 1 : 0) + 11;
        break;
    case 13:
        result = 13;
        break;
    case 14:
        result = (C_008346e8_IsAltVariant(self) ? 1 : 0) + 14;
        break;
    case 16:
        result = 16;
        break;
    case 17:
        result = 17;
        break;
    case 18:
        result = 18;
        break;
    case 19:
        result = 19;
        break;
    case 20:
        result = 20;
        break;
    case 21:
        result = 21;
        break;
    case 22:
        result = 22;
        break;
    default:
        break;
    }
    return result;
}

HOOK(0x004A6820, C_008346e8_MapIndex, "auto");
