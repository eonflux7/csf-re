#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());

// Returns true unless the mission-setup state is blocked: only mission id 7
// with a flag set in the global state and a cleared flag in the second global
// object prevents creating the state.
static bool __cdecl SceneMissionSetup_CanCreateState(int missionId)
{
    if (missionId != 7)
        return true;

    uint8_t* globalState = reinterpret_cast<uint8_t*>(GetGlobalState_50D190());
    if (CSF_AT(uint8_t, globalState, 0xD0) == 0)
        return true;

    uint8_t* globalSecond = reinterpret_cast<uint8_t*>(GetGlobalDword_73C020());
    if (CSF_AT(uint8_t, globalSecond, 0xFC) != 0)
        return true;

    return false;
}

HOOK(0x004F0B90, SceneMissionSetup_CanCreateState, "auto");
