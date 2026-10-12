#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, ObjectProvider_GetCurrentEntry, 0x004F4E90, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00861120_SetPendingRequest, 0x004F5EF0, (C_00861120* self, void* , int requestAddress));
CSF_FUNC(int, __fastcall, CharacterModel_SetUpdateMode, 0x004F7810, (void* self, void* , int mode));
CSF_FUNC(int, __stdcall, DevConsole_ParseArgument, 0x004FA800, (uint16_t* a1, uint16_t* a2));
CSF_FUNC(char, __fastcall, DevConsole_DisplayMessage, 0x004FAEA0, (void* self, void* edx, int a2, wchar_t* Source));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int a1, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall S_004faea0_SetCharacterUpdateMode(
    S_004faea0* self, void* /*edx*/, uint16_t* argument)
{
    alignas(8) uint8_t argumentBytes[0xC8];
    alignas(8) uint8_t requestBytes[0xC];

    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    auto* provider = static_cast<C_00861120*>(
        GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr));

    auto* parsedArgument = reinterpret_cast<uint16_t*>(argumentBytes);
    DevConsole_ParseArgument(argument, parsedArgument);

    uint16_t firstCharacter = parsedArgument[0];
    bool useSceneRequest = firstCharacter == 48;
    if (firstCharacter == 0)
    {
        char* currentEntry = ObjectProvider_GetCurrentEntry(provider, nullptr);
        useSceneRequest = CSF_AT(int, currentEntry, 0x8) < 0;
    }

    const char* message;
    if (useSceneRequest)
    {
        uint32_t context = GetSceneMenuContext(g_pCurrentScene, nullptr);
        C_00861120_SetPendingRequest(provider, nullptr, static_cast<int>(context + 28));
        CharacterModel_SetUpdateMode(provider, nullptr, 0);
        message = CSF_STR(0x0083F290, "CY02");
    }
    else
    {
        CSF_AT(void**, requestBytes, 0x0) = CSF_VTBL(0x0082D448);
        CSF_AT(int, requestBytes, 0x4) = -1;
        CSF_AT(int, requestBytes, 0x8) = -1;
        C_00861120_SetPendingRequest(
            provider, nullptr, reinterpret_cast<int>(requestBytes));
        CharacterModel_SetUpdateMode(provider, nullptr, 4);
        message = CSF_STR(0x0083F288, "CY01");
    }

    int displayText = Text_ConvertToDisplayEncoding(
        reinterpret_cast<int>(message), 16);

    // The packet also classifies this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x004FAEA0);
    return DevConsole_DisplayMessage(
        self, nullptr, 2, reinterpret_cast<wchar_t*>(displayText));
}

HOOK(0x004FB040, S_004faea0_SetCharacterUpdateMode, "auto");
