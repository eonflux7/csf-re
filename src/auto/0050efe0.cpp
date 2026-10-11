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

CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int*, __fastcall, ProfileSavegames_GetScenarioCollection, 0x00515CE0, (void* self, void*));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Ensures a "Demo" scenario entry exists when the profile has no savegame scenarios.
static int BddLoader_EnsureDemoScenarioWhenNoSavegames()
{
    int result = GetGlobalValue_75FDF0();
    void* globalsObj = reinterpret_cast<void*>(result);

    if (CSF_AT(uint8_t, globalsObj, 0xCC) == 0) {
        void* profileObj = reinterpret_cast<void*>(GetGlobalValue_8B056C());
        void* playerObj = CSF_AT(void*, profileObj, 8);

        int* scenarioCollection = ProfileSavegames_GetScenarioCollection(playerObj, nullptr);
        int scenarioBegin = CSF_AT(int, scenarioCollection, 4);
        if (scenarioBegin != 0) {
            int scenarioEnd = CSF_AT(int, scenarioCollection, 8);
            result = (scenarioEnd - scenarioBegin) / 80;
            if (result != 0) {
                return result;
            }
        }

        // No scenarios: build the "Demo" string and hand it to the player's virtual slot 0x54.
        alignas(8) uint8_t formatterBytes[0x20];
        FormatterSetPattern(formatterBytes, nullptr, CSF_STR(0x008405D4, "Demo"));
        result = ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, playerObj, 0)[0x54 / 4])(
            playerObj, nullptr, formatterBytes);

        if (CSF_AT(uint32_t, formatterBytes, 0x18) >= 8) {
            void* block = CSF_AT(void*, formatterBytes, 4);
            result = reinterpret_cast<int>(block);
            if (block != nullptr) {
                return Mem_FreeDebug(block);
            }
        }
    }
    return result;
}

HOOK(0x0050EFE0, BddLoader_EnsureDemoScenarioWhenNoSavegames, "auto");
