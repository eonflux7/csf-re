#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns one of four stored dwords selected by a key in [17, 20], or -1 for any other key.
static int __fastcall C_0083e7d8_GetValueForKey(C_0083e7d8* self, void* /*edx*/, int key) {
    int result = -1;
    switch (key) {
    case 17:
        result = static_cast<int>(self->field_0018);
        break;
    case 18:
        result = static_cast<int>(self->field_001c);
        break;
    case 19:
        result = static_cast<int>(self->field_0020);
        break;
    case 20:
        result = static_cast<int>(self->field_0024);
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x004E44D0, C_0083e7d8_GetValueForKey, "auto");
