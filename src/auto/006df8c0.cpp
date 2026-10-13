#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00861988_UpdateWaterEntryFromName, 0x007029D0, (C_00861988* self, void* /*edx*/, uint8_t* stringObj));

static char __fastcall C_00861988_UpdateWaterEntryIfReady(C_00861988* self, void* /*edx*/, int a2)
{
    char result = static_cast<char>(self->field_004c);
    if (result)
    {
        C_00861988* target = reinterpret_cast<C_00861988*>(self->field_0044);
        if (target)
            return C_00861988_UpdateWaterEntryFromName(target, nullptr, reinterpret_cast<uint8_t*>(a2));
    }
    return result;
}

HOOK(0x006DF8C0, C_00861988_UpdateWaterEntryIfReady, "auto");
