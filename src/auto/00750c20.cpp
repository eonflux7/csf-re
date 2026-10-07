#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static const char* __fastcall C_00868968_GetNetChatName(
    C_00868968* /*self*/, void* /*edx*/)
{
    return CSF_STR(0x0086893C, "NetChat");
}

HOOK(0x00750C20, C_00868968_GetNetChatName, "auto");
