#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0050D190: int __cdecl GetGlobalState(void)
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x0050D190, ());

// Global string pointers used for the BotonControles label.
CSF_GLOBAL(const char*, g_pszMenu_P_MeJueg, 0x00899AF4);
CSF_GLOBAL(const char*, g_pszMenu_P_MeJuMP, 0x00899AF8);

// Refreshes the controls button of the options menu: asks the owning
// menu object (field +0x14) to run its virtual slot 0x60 with (self, 0),
// then picks the button text depending on the global state flag at +0xD0
// and forwards it to the owner's virtual slot 0x58.
static int __fastcall CMenuOptionsPC_RefreshBotonControlesLabel(CMenuOptionsPC* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);

    void** ownerVtbl = CSF_AT(void**, owner, 0);
    ((void(__fastcall*)(void*, void*, CMenuOptionsPC*, int))ownerVtbl[0x60 / 4])(owner, nullptr, self, 0);

    uint8_t* globalState = reinterpret_cast<uint8_t*>(GetGlobalState());
    uint8_t stateFlag = CSF_AT(uint8_t, globalState, 0xD0);

    const char* labelText = g_pszMenu_P_MeJuMP;
    if (!stateFlag)
        labelText = g_pszMenu_P_MeJueg;

    void** ownerVtblAgain = CSF_AT(void**, owner, 0);
    return ((int(__fastcall*)(void*, void*, const char*))ownerVtblAgain[0x58 / 4])(owner, nullptr, labelText);
}

HOOK(0x0053B850, CMenuOptionsPC_RefreshBotonControlesLabel, "auto");
