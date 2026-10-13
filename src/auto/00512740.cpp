#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, SaveGame_DestroyRecord, 0x00500A80, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, S_004f21d0_ResetInnerBuffer, 0x00501170, (S_004f21d0* self, void*));
CSF_FUNC(int, __fastcall, CSaveGameManager_EndLoad, 0x00511FF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CSaveGameManager_BeginLoad, 0x005120A0, (void* self, void* edx, int a2, int a3, char a4));
CSF_FUNC(char, __fastcall, SceneLoader_FinalizeReferenceArchive, 0x00691E00, (void* self, void* edx));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(char, __fastcall, SceneLoader_ProcessDeferredReferences, 0x0069A910, (void* self, void* edx, int a2));

static bool __fastcall CSaveGameManager_LoadGame(
    CSaveGameManager* self, void* /*edx*/, int saveIndex)
{
    alignas(8) std::uint8_t recordBytes[0xC8];
    alignas(8) std::uint8_t contextBytes[0x28];
    S_004f21d0& record = *reinterpret_cast<S_004f21d0*>(recordBytes);

    CSF_AT(char, contextBytes, 0x24) = GetOptionsByteAt2();
    if (CSF_AT(char, contextBytes, 0x24))
        SceneLoader_SetFileOption(0);

    CSF_AT(char, contextBytes, 0x00) = 0;
    CSF_AT(int, contextBytes, 0x04) = 0;
    CSF_AT(int, contextBytes, 0x08) = 0;
    CSF_AT(int, contextBytes, 0x0C) = 0;
    CSF_AT(char, contextBytes, 0x14) = 0;
    CSF_AT(char, contextBytes, 0x15) = 0;
    CSF_AT(int, contextBytes, 0x18) = 100;
    CSF_AT(int, contextBytes, 0x1C) = 0;
    CSF_AT(S_004f21d0*, contextBytes, 0x20) = nullptr;

    if (!CSaveGameManager_BeginLoad(
            self, nullptr, reinterpret_cast<int>(contextBytes), saveIndex, 0))
    {
        SceneLoader_SetFileOption(CSF_AT(char, contextBytes, 0x24));
        return false;
    }

    S_004f21d0_ResetInnerBuffer(&record, nullptr);
    CSF_AT(S_004f21d0*, contextBytes, 0x20) = &record;

    ((void (__fastcall*)(CSaveGameManager*, void*, int*))
        reinterpret_cast<void**>(self->vtbl)[0x40 / 4])(
            self, nullptr, reinterpret_cast<int*>(contextBytes));

    if (CSF_AT(char, contextBytes, 0x00) == 0)
    {
        SceneLoader_FinalizeReferenceArchive(
            CSF_AT(void*, contextBytes, 0x08), nullptr);
        SceneLoader_ProcessDeferredReferences(
            CSF_AT(void*, contextBytes, 0x10), nullptr,
            reinterpret_cast<int>(contextBytes));
    }

    CSaveGameManager_EndLoad(self, nullptr);
    SceneLoader_SetFileOption(CSF_AT(char, contextBytes, 0x24));

    const bool success = CSF_AT(char, contextBytes, 0x00) == 0;
    SaveGame_DestroyRecord(reinterpret_cast<std::uint32_t*>(recordBytes), nullptr);
    return success;
}

HOOK(0x00512740, CSaveGameManager_LoadGame, "auto");
