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

// Callee: CMenuMpStats handler (ret 4, __fastcall: ecx = this, one stack arg).
CSF_FUNC(char, __fastcall, sub_747F60, 0x00747F60, (void* self, void* edx, char a2));

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

// Forwards the input character to the multiplayer stats menu, if one is open.
// The stats object is stored at state+4, and its CMenuMpStats base sits 0x308 bytes before it.
static char __stdcall MpSpawn_ForwardInputToMpStatsMenu(char a1)
{
    uint32_t selectStateAddress = g_CharacterSelectState8B0694;
    uint32_t statsMenuAddress = CSF_AT(uint32_t, reinterpret_cast<void*>(selectStateAddress), 4);

    if (statsMenuAddress != 0) {
        void* statsMenu = reinterpret_cast<void*>(statsMenuAddress - 0x308);
        return sub_747F60(statsMenu, nullptr, a1);
    }
    return sub_747F60(nullptr, nullptr, a1);
}

HOOK(0x00551700, MpSpawn_ForwardInputToMpStatsMenu, "auto");
