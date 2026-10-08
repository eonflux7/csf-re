#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// 0x0053A110 - __thiscall(int this, int a2), ret 4 - handles the command payload
CSF_FUNC(void*, __fastcall, CMenuPause_DispatchCommand, 0x53A110, (CMenuPause* self, void* /*edx*/, uint32_t* a2));

// 0x0053A4E0 - __thiscall(void* this, uint32_t* a2), ret 4
// Inspects the first dword of the incoming command; only id 13 is forwarded to the handler,
// otherwise the command pointer itself is returned untouched.
static void* __fastcall CMenuPause_HandleCommand(CMenuPause* self, void* /*edx*/, uint32_t* a2)
{
    if (*a2 == 13)
        return CMenuPause_DispatchCommand(self, nullptr, a2);
    return a2;
}

HOOK(0x53A4E0, CMenuPause_HandleCommand, "auto");
