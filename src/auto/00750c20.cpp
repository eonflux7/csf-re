#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static const char* __fastcall NetChat_GetNetChatName(
    NetChat* /*self*/, void* /*edx*/)
{
    return CSF_STR(0x0086893C, "NetChat");
}

HOOK(0x00750C20, NetChat_GetNetChatName, "auto");
