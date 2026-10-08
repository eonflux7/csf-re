#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// State used by the MP character-select menu to detect selection changes
// (drives the camera preview refresh).
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x8B0694);
CSF_GLOBAL(uint32_t, g_MenuMPCharacterSelect_LastState, 0x8B0698);

CSF_FUNC(int, __cdecl, GetGlobalState, 0x50D190, ());
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x550CC0, ());

// 0x542620 (tail-merges into the state comparison routine at 0x551DD0).
static bool __cdecl MenuMPCharacterSelect_HasStateChanged()
{
    uint8_t* globalState = reinterpret_cast<uint8_t*>(GetGlobalState());
    if (CSF_AT(uint8_t, globalState, 0xD0) == 0)
        return false;

    uint8_t* modeObject = reinterpret_cast<uint8_t*>(GetGlobal8B0688());
    if (CSF_AT(uint8_t, modeObject, 0x14A2) != 0)
        return true;

    uint8_t* selectionState = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(g_CharacterSelectState8B0694));
    if (selectionState == nullptr)
        return false;

    uint8_t* stateEntry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, selectionState, 4)));
    if (stateEntry == nullptr)
        return false;

    uint8_t* previewObject = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(stateEntry) - 776);
    if (previewObject == nullptr)
        return false;

    uint32_t lastState = g_MenuMPCharacterSelect_LastState;
    if (lastState == 0)
    {
        g_MenuMPCharacterSelect_LastState = CSF_AT(uint32_t, previewObject, 0x2B8);
        return false;
    }

    return CSF_AT(uint32_t, previewObject, 0x2B8) != lastState;
}

HOOK(0x542620, MenuMPCharacterSelect_HasStateChanged, "auto");
