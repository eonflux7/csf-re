#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char*, __fastcall, sub_6B29A0, 0x006B29A0, (void* self, void* edx));
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

// Returns a pointer 7 bytes into the name text of the selected character entry,
// or nullptr when no entry is selected (field +4 is 0 or the sentinel 776).
static char* __cdecl MpSpawn_GetCharacterSelectNamePtr() {
    uint32_t selectedEntryRaw = *reinterpret_cast<uint32_t*>(g_CharacterSelectState8B0694 + 4);
    if (selectedEntryRaw == 0 || selectedEntryRaw == 776) {
        return nullptr;
    }
    C_00860288* entry = reinterpret_cast<C_00860288*>(selectedEntryRaw - 776);
    return sub_6B29A0(entry, nullptr) + 7;
}

HOOK(0x00551620, MpSpawn_GetCharacterSelectNamePtr, "auto");
