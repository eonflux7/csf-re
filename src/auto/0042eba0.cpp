#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CHud_GetIndexedValue(CHud* self, void* /*edx*/, int entry)
{
    int index;
    switch (entry) {
    case 1: index = 0; break;
    case 2: index = 1; break;
    case 3: index = 2; break;
    case 4: index = 3; break;
    case 5: index = 4; break;
    default: return 0;
    }

    return reinterpret_cast<const int*>(self->field_01f8)[index];
}

HOOK(0x0042EBA0, CHud_GetIndexedValue, "auto");
