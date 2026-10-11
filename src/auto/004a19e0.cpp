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

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Looks up the associated HUD alert object owned by this->[2]->[0x18] and tail-jumps
// into its vtable slot 3 (offset 0x0C), forwarding the caller's arguments unchanged.
static hook::Tail __fastcall HudAlertIcons_UpdateAssociatedState(void* self, const uint32_t* /*args*/)
{
    // result = GetGlobalState();
    const uint32_t globalState = static_cast<uint32_t>(GetGlobalState_50D190());

    // if ( *(std::uint8_t *)(result + 208) )
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(globalState), 0xD0) == 0)
        return hook::tail_ret(globalState, 4);

    // result = *(std::uint32_t *)(*(this + 2) + 24);
    const uint32_t owner = CSF_AT(uint32_t, self, 0x08);
    const uint32_t inner = CSF_AT(uint32_t, reinterpret_cast<void*>(owner), 0x18);
    if (inner == 0)
        return hook::tail_ret(inner, 4);

    // result -= 1712;  if ( result )
    const uint32_t adjusted = inner - 0x6B0;
    if (adjusted == 0)
        return hook::tail_ret(adjusted, 4);

    // return (*(... **)(*(result + 1712) + 12))(result + 1712, a2);
    void* obj = reinterpret_cast<void*>(adjusted + 0x6B0);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0xC / 4]);
}

HOOK_TAIL(0x004A19E0, HudAlertIcons_UpdateAssociatedState, "hud_alert_icons");
