#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __cdecl, C_0083e500_GetValue, 0x0070DDF0, (void* widgetTarget, void* globalSetting));

// Original: push esi; esi = this; call GetGlobalValue; push [eax+0xF0]; push [esi+4]; call C_0083e500_GetValue; add esp,8; pop esi; ret
static int __fastcall HudQueryWidgetValue(void* self, void* /*edx*/)
{
    uint8_t* globalValue = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
    uint32_t widgetTarget = CSF_AT(uint32_t, self, 0x04);
    uint32_t globalSetting = CSF_AT(uint32_t, globalValue, 0xF0);
    return C_0083e500_GetValue(reinterpret_cast<void*>(widgetTarget),
                               reinterpret_cast<void*>(globalSetting));
}

HOOK(0x0042D3C0, HudQueryWidgetValue, "auto");
