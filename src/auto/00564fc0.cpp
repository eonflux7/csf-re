#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// int __cdecl GetGlobalState_50D190() at 0x50D190
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x50D190, ());

// const char* g_pszMenu_P_LbLaWa at 0x00899B74
CSF_GLOBAL(const char*, g_pszMenu_P_LbLaWa, 0x899B74);

// Clears the global menu selector (globalState->field_00CC = -1), then
// re-syncs the child label object stored at this+0x38: it is first reset
// with (this, 0) via vtbl[0x60/4] and then re-labelled with
// g_pszMenu_P_LbLaWa via vtbl[0x58/4]. Returns the second call's result.
static int __fastcall MenuAccounts_ResetAndApplyLabel(void* self, void* /*edx*/)
{
    *reinterpret_cast<int*>(GetGlobalState_50D190() + 0xcc) = -1;

    void* labelObject = *reinterpret_cast<void**>(static_cast<uint8_t*>(self) + 0x38);
    void** labelVtbl = *reinterpret_cast<void***>(labelObject);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*, int)>(labelVtbl[0x60 / 4])(labelObject, nullptr, self, 0);

    void* labelObject2 = *reinterpret_cast<void**>(static_cast<uint8_t*>(self) + 0x38);
    void** labelVtbl2 = *reinterpret_cast<void***>(labelObject2);
    return reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(labelVtbl2[0x58 / 4])(labelObject2, nullptr, g_pszMenu_P_LbLaWa);
}

HOOK(0x564FC0, MenuAccounts_ResetAndApplyLabel, "auto");
