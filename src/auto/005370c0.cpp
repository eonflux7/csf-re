#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, AppMain_AppendPageNameAndRefreshDisplay, 0x00507570, (int owner, char* format, ...));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __fastcall, C_0085e7b8_RunMissionSetupLoader, 0x004E9C90, (C_0085e7b8* self, void* edx));

static int __cdecl GameBddLoader_FinalizeLoading()
{
    CoreFiles_SetFileOption(0);

    // Scene name holder: the name slot sits at +124; its length is at +0x94 (148).
    // Short names (length < 0x10) are stored inline at slot+4; longer names are referenced by pointer at slot+4.
    uint8_t* holder = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8B0568()));
    uint32_t nameLength = CSF_AT(uint32_t, holder, 0x94);
    uint8_t* nameSlot = holder + 124;
    const char* sceneName;
    if (nameLength < 0x10)
        sceneName = reinterpret_cast<const char*>(nameSlot + 4);
    else
        sceneName = CSF_AT(const char*, nameSlot, 4);

    int ownerPage = GetGlobalValue_8B056C();
    AppMain_AppendPageNameAndRefreshDisplay(ownerPage, const_cast<char*>(CSF_STR(0x841FCC, "INSTANCING SCENE: %s")),
                                            const_cast<char*>(sceneName));

    C_0085e7b8* missionSetup = reinterpret_cast<C_0085e7b8*>(static_cast<uintptr_t>(GetValue8B0554()));
    const char* resultText;
    if (C_0085e7b8_RunMissionSetupLoader(missionSetup, nullptr))
        resultText = CSF_STR(0x841FB4, "INSTANCING RESULT: OK");
    else
        resultText = CSF_STR(0x841F98, "INSTANCING RESULT: FAILED");

    int ownerPageForResult = GetGlobalValue_8B056C();
    AppMain_AppendPageNameAndRefreshDisplay(ownerPageForResult, const_cast<char*>(resultText));

    return static_cast<int>(reinterpret_cast<uintptr_t>(CoreFiles_SetFileOption(1)));
}

HOOK(0x005370C0, GameBddLoader_FinalizeLoading, "auto");
