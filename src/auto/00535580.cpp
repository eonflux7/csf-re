#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// 0x00535460 - __thiscall(CMenuMpPause* this, uint32_t* a3), ret 4
// Handles the command payload once it has been recognized.
CSF_FUNC(void*, __fastcall, CMenuMpPause_HandleButtonEvent, 0x535460, (CMenuMpPause* self, void* /*edx*/, uint32_t* a3));

// 0x00535580 - __thiscall(CMenuMpPause* this, uint32_t* a3), ret 4
// Inspects the first dword of the incoming command; only id 13 is forwarded to the
// handler, otherwise the command pointer itself is left in eax and returned untouched.
static void* __fastcall CMenuMpPause_HandleCommand(CMenuMpPause* self, void* /*edx*/, uint32_t* a3)
{
    if (*a3 == 13)
        return CMenuMpPause_HandleButtonEvent(self, nullptr, a3);
    return a3;
}

HOOK(0x535580, CMenuMpPause_HandleCommand, "auto");
