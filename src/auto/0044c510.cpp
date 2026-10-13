#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee bindings (by address).
CSF_FUNC(int, __cdecl, HudEntry_ClearSlot, 0x0044C4A0, (int* a1));
CSF_FUNC(unsigned int, __cdecl, Hud_UnregisterObject, 0x004754D0, (int a1));

// Clears the HUD registration of this object if it is active.
// Original: if (this->active[+8]) { HudEntry_ClearSlot(&this->slot[+12]); result = Hud_UnregisterObject(this); this->active = 0; }
static char __fastcall ReleaseHudRegistration(void* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 0x08);
    if (result)
    {
        HudEntry_ClearSlot(reinterpret_cast<int*>(static_cast<uint8_t*>(self) + 12));
        result = static_cast<char>(Hud_UnregisterObject(reinterpret_cast<int>(self)));
        CSF_AT(uint8_t, self, 0x08) = 0;
    }
    return result;
}

HOOK(0x0044C510, ReleaseHudRegistration, "auto");
