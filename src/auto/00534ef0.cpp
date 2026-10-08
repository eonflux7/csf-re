#include <cstdint>
#include "hook.h"
#include "globals.h"

CSF_FUNC(uint8_t, __cdecl, MenuQueryArgumentFlag, 0x007A8ED0, (int argument));
CSF_FUNC(void, __fastcall, MenuMpPause_SetFlagState, 0x00534AE0, (void* self, void* edx, int enabled));
CSF_FUNC(int, __cdecl, MenuHandleCommandId, 0x00428500, (int commandId));

static int __fastcall MenuMpPause_ForwardCommandWithFlag(void* self, void* /*edx*/, int commandId, int /*unusedArg*/)
{
    if (MenuQueryArgumentFlag(commandId))
        MenuMpPause_SetFlagState(reinterpret_cast<uint8_t*>(self) - 4, nullptr, 1);
    return MenuHandleCommandId(commandId);
}

HOOK(0x00534EF0, MenuMpPause_ForwardCommandWithFlag, "auto");
