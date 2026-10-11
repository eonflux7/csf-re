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

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

CSF_FUNC(char*, __fastcall, GetSelectionRecordFromEntry, 0x006B29A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ProcessTeamSpawnRequest, 0x005561D0, (void* self, void* edx, float a2));

// Looks up the selection entry linked from the global character-select state; if that
// entry's record type byte is 3, forwards the request to the team-spawn handler.
static int __fastcall MpSpawn_ForwardIfSelectionTypeIs3(S_00555e40* self, void* /*edx*/, float a2)
{
    int result = static_cast<int>(g_CharacterSelectState8B0694);
    if (g_CharacterSelectState8B0694) {
        void* selectionState = reinterpret_cast<void*>(g_CharacterSelectState8B0694);
        result = CSF_AT(int, selectionState, 4);
        if (result) {
            if (result != 776) {
                char* record = GetSelectionRecordFromEntry(reinterpret_cast<void*>(result - 776), nullptr);
                result = static_cast<int>(reinterpret_cast<uintptr_t>(record));
                if (static_cast<uint8_t>(record[6]) == 3)
                    return ProcessTeamSpawnRequest(self, nullptr, a2);
            }
        }
    }
    return result;
}

HOOK(0x00556E40, MpSpawn_ForwardIfSelectionTypeIs3, "auto");
