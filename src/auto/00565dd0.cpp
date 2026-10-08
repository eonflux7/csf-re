// Target: 0x00565DD0 (CMenuMpGameMode, Enter-key handling over four menu slots)
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

// Callees (bound by address)
CSF_FUNC(int, __fastcall, CMenuMpGameMode_ApplyDeathmatchSelection, 0x00565CF0, (CMenuMpGameMode* self, void* edx, int slotIndex));
CSF_FUNC(int, __fastcall, CMenuMpGameMode_SelectDeathMatchMode, 0x00565CB0, (CMenuMpGameMode* self, void* edx));

// Handles a key event: when the event code is 13 and the event's second dword
// matches a non-null slot handle in the four-entry array at +0x20, activates
// that slot. Slots 0-2 take their index; slot 3 uses the separate handler.
static void __fastcall CMenuMpGameMode_OnEnterKeyActivateSlot(CMenuMpGameMode* self, void* /*edx*/, uint32_t* keyEvent)
{
    if (keyEvent[0] != 13)
        return;

    for (int slot = 0; slot < 4; ++slot)
    {
        uint32_t slotHandle = CSF_AT(uint32_t, self, 0x20 + slot * 4);
        if (slotHandle != 0 && slotHandle == keyEvent[1])
        {
            if (slot <= 2)
                CMenuMpGameMode_ApplyDeathmatchSelection(self, nullptr, slot);
            else
                CMenuMpGameMode_SelectDeathMatchMode(self, nullptr);
        }
    }
}

HOOK(0x00565DD0, CMenuMpGameMode_OnEnterKeyActivateSlot, "auto");
