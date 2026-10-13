#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int*, __fastcall, ProfileSavegames_GetScenarioCollection, 0x00515CE0, (void* self, void*));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x00716A60, (CGuiListBoxBase* self, void*, int index));
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);

static uint8_t* GetGlobalObject() {
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8B056C()));
}

// Resets the game-mode list and the list box, then adds every mode entry whose
// flag matches the current MP network type, and finally refreshes the scenario source.
static int __fastcall CMenuMpGameMode_PopulateModesForNetworkType(void* self, void* /*edx*/) {
    uint8_t* globalObj = GetGlobalObject();
    void* modeManager = CSF_AT(void*, globalObj, 0x14);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, modeManager, 0)[10])(modeManager, nullptr);

    void* listBox = CSF_AT(void*, self, 0x14);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, listBox, 0)[166])(listBox, nullptr);

    int matchingCount = 0;
    for (int i = 0; i < 10; ++i) {
        uint8_t* globalObjIter = GetGlobalObject();
        void* entryManager = CSF_AT(void*, globalObjIter, 0x14);
        uint8_t* entry = reinterpret_cast<uint8_t*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, entryManager, 0)[12])(entryManager, nullptr, i);
        if (entry != nullptr && static_cast<int>(*entry) == ((g_MpNetworkType == 2) ? 1 : 0)) {
            reinterpret_cast<void(__fastcall*)(void*, void*, int, int, uint8_t*)>(
                CSF_AT(void**, self, 0)[25])(self, nullptr, matchingCount, i, entry);
            ++matchingCount;
        }
    }

    CGuiListBoxBase* selectedListBox = CSF_AT(CGuiListBoxBase*, self, 0x14);
    CGuiListBoxBase_AddSelectedItem(selectedListBox, nullptr, 0);

    uint8_t* refreshGlobal = GetGlobalObject();
    void* refreshTarget = CSF_AT(void*, refreshGlobal, 0x08);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, refreshTarget, 0)[13])(refreshTarget, nullptr);

    uint8_t* scenarioGlobal = GetGlobalObject();
    void* scenarioSource = CSF_AT(void*, scenarioGlobal, 0x08);
    return static_cast<int>(reinterpret_cast<intptr_t>(
        ProfileSavegames_GetScenarioCollection(scenarioSource, nullptr)));
}

HOOK(0x564F00, CMenuMpGameMode_PopulateModesForNetworkType, "auto");
