#include <cstdint>
#include "globals.h"
#include "hook.h"

static hook::Tail __fastcall DevConsole_ForwardPendingMenuUpdate(
    void* self, const uint32_t* args)
{
    (void)args;

    uint32_t pendingMenu = CSF_AT(uint32_t, self, 0x4);
    if (pendingMenu == 0)
        return hook::tail_ret(0u, 4);

    void* obj = reinterpret_cast<void*>(pendingMenu);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x5C / 4]);
}

HOOK_TAIL(0x004FB920, DevConsole_ForwardPendingMenuUpdate, "dev_console");
