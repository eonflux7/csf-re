#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// 0x0053A6C0 - __thiscall(CMenuMain* this, int a2), ret 4 - handles the command payload
CSF_FUNC(int, __fastcall, CMenuMain_DispatchButtonEvent, 0x53A6C0, (CMenuMain* self, void* /*edx*/, uint32_t* a2));

// 0x0053A920 - __thiscall(CMenuMain* this, uint32_t* a2), ret 4
// Inspects the first dword (command id) of the incoming payload; only id 13 is forwarded to
// the handler, otherwise the payload pointer itself is returned untouched.
static int __fastcall CMenuMain_HandleCommand(CMenuMain* self, void* /*edx*/, uint32_t* a2)
{
    if (*a2 == 13)
        return CMenuMain_DispatchButtonEvent(self, nullptr, a2);
    return reinterpret_cast<int>(a2);
}

HOOK(0x53A920, CMenuMain_HandleCommand, "auto");
