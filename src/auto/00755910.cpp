#include "hook.h"

static int __cdecl IsSpecialTypeCode(int code)
{
    return code == 8 || code == 9 || code == 10 || code == 16 || code == 17;
}

HOOK(0x755910, IsSpecialTypeCode, "auto");
