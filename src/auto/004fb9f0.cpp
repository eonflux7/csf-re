#include <cstdint>
#include "globals.h"
#include "hook.h"

// Forwards a stop request to the child object at this+8 through its vtable slot 13
// (byte offset 0x34). Returns 0 directly when either child pointer is empty.
static hook::Tail __fastcall MenuExtras_StopGlobalPlayback(void* self, const uint32_t* args)
{
    (void)args;

    uint32_t firstChild = CSF_AT(uint32_t, self, 0x04);
    if (firstChild != 0)
    {
        uint32_t secondChild = CSF_AT(uint32_t, self, 0x08);
        if (secondChild != 0)
        {
            void* obj = reinterpret_cast<void*>(secondChild);
            return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x34 / 4]);
        }
    }
    return hook::tail_ret(0, 4);
}

HOOK_TAIL(0x004FB9F0, MenuExtras_StopGlobalPlayback, "dev_console");
