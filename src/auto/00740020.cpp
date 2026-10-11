#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetPlayerEntryId, 0x0073D0F0, (S_0073ecc0* self, void*));
CSF_FUNC(char, __fastcall, sub_742880, 0x00742880, (void* self, void* edx, char a2, char a3));

static void __fastcall CMenuMpStats_HandlePlayerSpawnEvent(void* self, void* /*edx*/, uint16_t playerSlot, int playerEntryId)
{
    if (CSF_AT(float, self, 0xB0) < g_fZero)
    {
        if (Sub_64B250(self, nullptr))
        {
            sub_742880(self, nullptr, (char)playerSlot, (char)playerEntryId);
            CSF_AT(float, self, 0xB0) = 1.0f;
        }
        S_0073ecc0* localPlayerState = reinterpret_cast<S_0073ecc0*>(GetGlobalDword_73C020());
        if (playerEntryId == MpSpawnPoints_GetPlayerEntryId(localPlayerState, nullptr))
            CSF_AT(int, self, 0xA8) = (int)playerSlot;
    }
}

HOOK(0x740020, CMenuMpStats_HandlePlayerSpawnEvent, "auto");
