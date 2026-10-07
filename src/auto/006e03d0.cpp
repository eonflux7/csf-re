#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Reads the byte flag at +0x08. If it is non-zero the flag is cleared and the
// previous (non-zero) value is returned; otherwise 0 is returned unchanged.
static char __fastcall C_008619e0_ConsumeFlag0008(C_008619e0* self, void* /*edx*/)
{
    char previousFlag = static_cast<char>(self->field_0008);
    if (previousFlag)
        self->field_0008 = 0;
    return previousFlag;
}

HOOK(0x006E03D0, C_008619e0_ConsumeFlag0008, "auto");
