#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall C_00860e68_SetVisibleFlag(
    C_00860e68* self, void* /*edx*/, char flag)
{
    if (self->field_0018 != static_cast<uint8_t>(flag))
        self->field_0018 = static_cast<uint8_t>(flag);
    return flag;
}

HOOK(0x006D0DF0, C_00860e68_SetVisibleFlag, "auto");
