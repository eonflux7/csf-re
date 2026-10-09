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

CSF_FUNC(int, __fastcall, SceneEntryIndexFromFloat, 0x00468E20, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GlobalTableHasEntry, 0x004FC140, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void, __fastcall, GlobalTableSetEntry, 0x004FCA10, (void* self, void* edx, int a2, int a3));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Follows slotValue -> record; record+4 -> inner record; inner+4 holds the float.
static float ReadNestedFloat(uint32_t slotValue)
{
    uint8_t* record = reinterpret_cast<uint8_t*>(slotValue);
    uint32_t innerValue = *reinterpret_cast<uint32_t*>(record + 4);
    uint8_t* inner = reinterpret_cast<uint8_t*>(innerValue);
    return *reinterpret_cast<float*>(inner + 4);
}

static void __stdcall ApplyFloatPairToSceneEntry(int, int, uint32_t* slotArray)
{
    float firstValue = ReadNestedFloat(slotArray[0]);
    int32_t firstInt = static_cast<int32_t>(static_cast<int64_t>(firstValue));

    uint8_t* sceneEntry = static_cast<uint8_t*>(CSF_AT(void*, g_pCurrentScene, 0xBC)) + 0x94;
    int sceneIndex = SceneEntryIndexFromFloat(sceneEntry, nullptr, firstInt);
    if (sceneIndex == -1)
        return;

    void* globalTable = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    if (GlobalTableHasEntry(globalTable, nullptr, static_cast<unsigned int>(sceneIndex)))
    {
        float secondValue = ReadNestedFloat(slotArray[1]);
        int32_t secondInt = static_cast<int32_t>(static_cast<int64_t>(secondValue));

        void* globalTableForSet = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
        GlobalTableSetEntry(globalTableForSet, nullptr, sceneIndex, secondInt);
    }
}

HOOK(0x0045DDF0, ApplyFloatPairToSceneEntry, "auto");
