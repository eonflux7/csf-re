#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __fastcall, C_0085e7b8_RunMissionSetupLoader, 0x004E9C90, (C_0085e7b8* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, AppMain_AppendPageNameAndRefreshDisplay, 0x00507570, (int owner, char* format, ...));
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));

static int __cdecl Interface_FinalizeSceneInstancing()
{
    CoreFiles_SetFileOption(0);

    auto* sceneBlock = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(GetGlobalValue_8B0568()));
    const uint32_t capacity = CSF_AT(uint32_t, sceneBlock, 148);
    uint8_t* sceneString = sceneBlock + 124;

    const char* sceneName;
    if (capacity < 0x10)
        sceneName = reinterpret_cast<const char*>(sceneString + 4);
    else
        sceneName = CSF_AT(const char*, sceneString, 4);

    int owner = GetGlobalValue_8B056C();
    AppMain_AppendPageNameAndRefreshDisplay(
        owner,
        const_cast<char*>(CSF_STR(0x00841FCC, "INSTANCING SCENE: %s")),
        sceneName);

    auto* loader = reinterpret_cast<C_0085e7b8*>(
        static_cast<uintptr_t>(GetValue8B0554()));
    const char* resultText;
    if (C_0085e7b8_RunMissionSetupLoader(loader, nullptr))
        resultText = CSF_STR(0x00841FB4, "INSTANCING RESULT: OK");
    else
        resultText = CSF_STR(0x00841F98, "INSTANCING RESULT: FAILED");

    owner = GetGlobalValue_8B056C();
    // The original pushes exactly two words here (owner, resultText) and its
    // `add esp, 8` pops only those. Because the callee is variadic, the emulator
    // reads a third stack word too: the original's is the word at this frame's
    // entry stack slot, the return address of the caller (`call` at push time
    // leaves it above the two pushes). The result format has no conversion, so
    // the callee never reads it. Pass that same word so the call is identical.
    const int callerReturn = static_cast<int>(
        reinterpret_cast<uintptr_t>(__builtin_return_address(0)));
    AppMain_AppendPageNameAndRefreshDisplay(
        owner, const_cast<char*>(resultText), callerReturn);

    return static_cast<int>(
        reinterpret_cast<uintptr_t>(CoreFiles_SetFileOption(1)));
}

HOOK(0x005370C0, Interface_FinalizeSceneInstancing, "auto");
