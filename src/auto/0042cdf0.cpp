#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(int, __fastcall, sub_428820, 0x00428820, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_42CA40, 0x0042CA40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());

// Checks the cheat gates for a CMenuControls-style screen. When both gates allow it,
// refreshes the menu state and tail-jumps into the global manager's child object.
static hook::Tail __fastcall C_00830578_TailEnterMenuState(void* self, const uint32_t* args)
{
    (void)args;
    C_00830578* menu = reinterpret_cast<C_00830578*>(self);

    int globalState = GetGlobalState_50D190();
    if (CSF_AT(uint8_t, globalState, 0xD0) != 0)
        return hook::tail_ret(0);

    int globalValue = GetGlobalValue_75FDF0();
    if (CSF_AT(uint8_t, globalValue, 0xCC) == 0)
        return hook::tail_ret(0);

    sub_428820(reinterpret_cast<void*>(menu->field_0014), nullptr);

    int globalValue8B0568 = GetGlobalValue_8B0568();
    if (CSF_AT(int, globalValue8B0568, 0x20) == 0)
        sub_42CA40(menu, nullptr);

    int manager = GetGlobalValue_8B056C();
    void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, manager, 0x04));
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x08 / 4]);
}

HOOK_TAIL(0x0042CDF0, C_00830578_TailEnterMenuState, "cheats");
