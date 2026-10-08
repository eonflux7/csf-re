#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// 0x00543490 - __thiscall(CMenuControls* this, uint32_t* a2), ret 4
// Handles the command payload when the id is 13.
CSF_FUNC(void*, __fastcall, CMenuControls_DispatchCommand, 0x543490, (CMenuControls* self, void* /*edx*/, uint32_t* a2));

// 0x005437F0 - __thiscall(CMenuControls* this, uint32_t* a2), ret 4
// Inspects the first dword of the incoming command; only id 13 is forwarded to the
// handler, otherwise the command pointer itself is returned untouched.
static void* __fastcall CMenuControls_HandleCommand(CMenuControls* self, void* /*edx*/, uint32_t* a2)
{
    if (*a2 == 13)
        return CMenuControls_DispatchCommand(self, nullptr, a2);
    return a2;
}

HOOK(0x5437F0, CMenuControls_HandleCommand, "auto");
