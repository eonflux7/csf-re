#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
// 0x70DDF0 is the shared `fld [0x82D4CC]; ret` stub (returns 0.0f in st0, ignores its arguments)
CSF_FUNC(float, __cdecl, C_0083e500_GetValue, 0x0070DDF0, (void* widgetTarget, void* globalSetting));

// Original: push esi; esi = this; call GetGlobalValue; push [eax+0xF0]; push [esi+4]; call 0x70DDF0; add esp,8;
// pop esi; ret -- the callee's st0 is the return value
static float __fastcall HudQueryWidgetValue(void* self, void* /*edx*/)
{
    uint8_t* globalValue = reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760());
    uint32_t widgetTarget = CSF_AT(uint32_t, self, 0x04);
    uint32_t globalSetting = CSF_AT(uint32_t, globalValue, 0xF0);
    return C_0083e500_GetValue(reinterpret_cast<void*>(widgetTarget),
                               reinterpret_cast<void*>(globalSetting));
}

HOOK(0x0042D3C0, HudQueryWidgetValue, "auto");
