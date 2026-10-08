#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original engine helpers, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(int, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(char, __fastcall, CMenuControls_RunActionChecked, 0x0042CDA0, (int** self, void* edx));
CSF_FUNC(int, __fastcall, Global8afc78_SetEnabled, 0x0042DFF0, (uint32_t* self, void* edx, char enabled));

// Attempts the menu action on this controller. Does nothing unless the global
// state flag at +0xD0 is clear and the global value flag at +0xCC is set.
// On success, forwards a cleared flag to the object returned by GetGlobal8afc78.
static char __fastcall CMenuControls_TryActivate(int** self, void* /*edx*/)
{
    uint8_t* globalState = reinterpret_cast<uint8_t*>(GetGlobalState_50D190());
    if (CSF_AT(uint8_t, globalState, 0xD0))
        return 0;

    uint8_t* globalValue = reinterpret_cast<uint8_t*>(GetGlobalValue_75FDF0());
    if (!CSF_AT(uint8_t, globalValue, 0xCC))
        return 0;

    char actionSucceeded = CMenuControls_RunActionChecked(self, nullptr);
    if (actionSucceeded)
    {
        uint32_t* target = reinterpret_cast<uint32_t*>(GetGlobal8afc78());
        Global8afc78_SetEnabled(target, nullptr, 0);
    }
    return actionSucceeded;
}

HOOK(0x00543AA0, CMenuControls_TryActivate, "auto");
