#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee at 0x6B29A0: __fastcall, takes the spawn-point object as `this`, returns a char*.
CSF_FUNC(char*, __fastcall, ResolveSpawnPointName, 0x006B29A0, (void* self, void* edx));

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

// Returns the name for the currently selected character's spawn point, or nullptr if
// there is no character-select state or no selected entry.
static char* GetSelectedCharacterSpawnName() {
    void* selectState = reinterpret_cast<void*>(g_CharacterSelectState8B0694);
    void* spawnObject = nullptr;

    if (g_CharacterSelectState8B0694 != 0) {
        uint32_t selectedEntry = CSF_AT(uint32_t, selectState, 4);
        if (selectedEntry != 0) {
            // The spawn object lives 0x308 bytes before the stored entry pointer.
            spawnObject = reinterpret_cast<void*>(selectedEntry - 0x308);
        }
    }

    char* result = nullptr;
    if (spawnObject != nullptr) {
        return ResolveSpawnPointName(spawnObject, nullptr);
    }
    return result;
}

HOOK(0x551310, GetSelectedCharacterSpawnName, "auto");
