#include <cstdint>
#include "hook.h"

// Returns the session-name text offset: the input value advanced by 6 bytes.
static int __cdecl NetDdlObjectManager_ConvertSessionNameText(int textOffset) {
    return textOffset + 6;
}

HOOK(0x00749720, NetDdlObjectManager_ConvertSessionNameText, "auto");
