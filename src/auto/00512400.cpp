#include <cstdint>
#include "globals.h"
#include <cstring>

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SceneLoader_SetFileOption, 0x00698600, (char a1));
CSF_FUNC(int, __fastcall, CSaveGameManager_PostLoadRefresh, 0x00511FF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CSaveGameManager_ReadSlotInto, 0x005120A0, (void* self, void* edx, int a2, int a3, char a4));
CSF_FUNC(int, __fastcall, SaveGameBuffer_Reset, 0x005144C0, (void* self, void* edx, int a2));

// __thiscall(this, a2, a3), ret 8. Loads a save slot into the manager and
// restores the scene loader option. Returns 1 on success, 0 on failure.
static char __fastcall CSaveGameManager_LoadSaveSlot(CSaveGameManager* self, void* /*edx*/, int a2, int a3)
{
    // Original stores the option byte in a local at [ebp-4].
    char savedSceneOption = GetOptionsByteAt2();
    if (savedSceneOption != 0)
        SceneLoader_SetFileOption(0);

    // 0x28-byte request buffer on the stack (original's local at [ebp-0x28]).
    alignas(8) uint8_t slotRequestBytes[0x28];
    CSF_AT(uint8_t, slotRequestBytes, 0x00) = 0;
    memset(slotRequestBytes + 4, 0, 12);
    CSF_AT(uint8_t, slotRequestBytes, 0x14) = 0;
    CSF_AT(uint8_t, slotRequestBytes, 0x15) = 0;
    CSF_AT(int, slotRequestBytes, 0x18) = 100;
    CSF_AT(int, slotRequestBytes, 0x1C) = 0;
    CSF_AT(int, slotRequestBytes, 0x20) = 0;

    int requestAddress = static_cast<int>(reinterpret_cast<uintptr_t>(slotRequestBytes));
    if (CSaveGameManager_ReadSlotInto(self, nullptr, requestAddress, a3, 0))
    {
        SaveGameBuffer_Reset(reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), nullptr, requestAddress);
        CSaveGameManager_PostLoadRefresh(self, nullptr);
        SceneLoader_SetFileOption(savedSceneOption);
        return 1;
    }

    SceneLoader_SetFileOption(savedSceneOption);
    return 0;
}

HOOK(0x00512400, CSaveGameManager_LoadSaveSlot, "auto");
