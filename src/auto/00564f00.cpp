#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x716A60, (void* self, void* edx, int itemIndex));
CSF_FUNC(int*, __fastcall, GetScenarioCollection, 0x515CE0, (void* self, void* edx));
CSF_GLOBAL(int, g_MpNetworkType, 0x89A5F0);

// Rebuilds the multiplayer game-mode list: resets the global list state, clears the list box,
// then adds every mode entry whose flag matches the current network type (MpNetworkType == 2 -> 1, else 0).
static int* __fastcall CMenuMpGameMode_PopulateModesForNetworkType(void* self, void* /*edx*/)
{
    // Global manager -> +0x14 sub-object: vtable slot 10 (+0x28), no args.
    void* globalManagerA = GetGlobalValue_8B056C();
    void* managerSubA = *CSF_AT(void**, globalManagerA, 0x14);
    void** vtableA = *CSF_AT(void***, managerSubA, 0);
    ((void(__fastcall*)(void*, void*))vtableA[10])(managerSubA, nullptr);

    // self +0x14 member object: vtable slot 166 (+0x298), no args.
    void* memberObject = *CSF_AT(void**, self, 0x14);
    void** memberVtable = *CSF_AT(void***, memberObject, 0);
    ((void(__fastcall*)(void*, void*))memberVtable[166])(memberObject, nullptr);

    int matchCount = 0;
    for (int modeIndex = 0; modeIndex < 10; ++modeIndex)
    {
        void* globalManagerB = GetGlobalValue_8B056C();
        void* managerSubB = *CSF_AT(void**, globalManagerB, 0x14);
        void** vtableB = *CSF_AT(void***, managerSubB, 0);
        uint8_t* modeEntry = reinterpret_cast<uint8_t*>(
            ((void*(__fastcall*)(void*, void*, int))vtableB[12])(managerSubB, nullptr, modeIndex));

        if (modeEntry != nullptr)
        {
            int entryFlag = *modeEntry;
            int wantedFlag = (g_MpNetworkType == 2) ? 1 : 0;
            if (entryFlag == wantedFlag)
            {
                void** selfVtable = *CSF_AT(void***, self, 0);
                ((void(__fastcall*)(void*, void*, int, int, uint8_t*))selfVtable[25])(
                    self, nullptr, matchCount, modeIndex, modeEntry);
                ++matchCount;
            }
        }
    }

    // Clear the list box owned at self +0x14 (argument 0).
    CGuiListBoxBase_AddSelectedItem(*CSF_AT(void**, self, 0x14), nullptr, 0);

    // Global manager -> +8 sub-object: vtable slot 13 (+0x34), no args.
    void* globalManagerC = GetGlobalValue_8B056C();
    void* managerSubC = *CSF_AT(void**, globalManagerC, 8);
    void** vtableC = *CSF_AT(void***, managerSubC, 0);
    ((void(__fastcall*)(void*, void*))vtableC[13])(managerSubC, nullptr);

    // Tail call: finish the refresh on the global manager's +8 object and return its result.
    void* globalManagerD = GetGlobalValue_8B056C();
    void* finishTarget = *CSF_AT(void**, globalManagerD, 8);
    return GetScenarioCollection(finishTarget, nullptr);
}

HOOK(0x00564F00, CMenuMpGameMode_PopulateModesForNetworkType, "auto");
