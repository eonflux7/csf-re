#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// 0x0052E1B0 - __thiscall(int this, int a2), ret 4 - handles the command payload
CSF_FUNC(void*, __fastcall, CMenuScenarioSelect_DispatchCommand, 0x52E1B0, (CMenuScenarioSelect* self, void* /*edx*/, uint32_t* a2));

// 0x0052E390 - __thiscall(void* this, uint32_t* a2), ret 4
// Inspects the first dword of the incoming command; only id 13 is forwarded to the handler,
// otherwise the command pointer itself is returned untouched.
static void* __fastcall CMenuScenarioSelect_HandleCommand(CMenuScenarioSelect* self, void* /*edx*/, uint32_t* a2)
{
    if (*a2 == 13)
        return CMenuScenarioSelect_DispatchCommand(self, nullptr, a2);
    return a2;
}

HOOK(0x52E390, CMenuScenarioSelect_HandleCommand, "auto");
