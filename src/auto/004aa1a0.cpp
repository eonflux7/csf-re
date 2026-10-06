#include "globals.h"
#include "hook.h"

static const char* __cdecl GetCoverFireTypeName()
{
    return CSF_STR(0x00837E54, "TFuegoCobertura");
}

HOOK(0x004AA1A0, GetCoverFireTypeName, "auto");
