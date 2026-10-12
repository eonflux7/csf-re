#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __stdcall, SceneMissionSetup_ReadOpeningVideoName, 0x004EE130, (const char* missionPath, void* outVideoName));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResetMenuResources, 0x0050E130, (void* self, void*));

static char __fastcall SceneResult_ResolveOpeningVideo(C_0082d410* self, void* /*edx*/)
{
    char result = 0;
    bool openingVideoFound = false;

    uint8_t* globalValue = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_75FDF0()));
    if (CSF_AT(uint8_t, globalValue, 0xCE) != 0)
    {
        int stateBaseValue = GetGlobalValue_8B0568();
        uint8_t* stateBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(stateBaseValue));
        uint32_t entryCount = CSF_AT(uint32_t, stateBase, 0x94);

        // Original: eax = base + 124; if entryCount < 0x10 -> &eax[4], else eax[4] is a pointer.
        uint8_t* slotBase = stateBase + 124;

        S_00693740* missionSetup;
        if (entryCount < 0x10)
            missionSetup = reinterpret_cast<S_00693740*>(slotBase + 4);
        else
            missionSetup = CSF_AT(S_00693740*, slotBase, 4);

        GetValue8B0554();
        result = SceneMissionSetup_ReadOpeningVideoName(
            reinterpret_cast<const char*>(missionSetup),
            reinterpret_cast<uint8_t*>(self) + 0x114);
        openingVideoFound = (result != 0);
    }

    if (openingVideoFound)
    {
        CSF_AT(uint32_t, self, 0x130) = 0;
    }
    else
    {
        int globalState = GetGlobalState_50D190();
        result = static_cast<char>(SceneLoader_ResetMenuResources(reinterpret_cast<void*>(static_cast<uintptr_t>(globalState)), nullptr));
        CSF_AT(uint32_t, self, 0x130) = 2;
    }

    return result;
}

HOOK(0x00536E20, SceneResult_ResolveOpeningVideo, "auto");
