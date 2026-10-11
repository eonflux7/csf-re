#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_0083e8c8_ApplyHandle, 0x004E1A10, (C_0083e8c8* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, C_0083e8c8_PrepareSpawnSelection, 0x004E1FE0, (C_0083e8c8* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, GetMpGameModeIndex, 0x005515F0, ());
CSF_FUNC(int, __fastcall, C_00844694_ForwardSpawnRequest, 0x00551680, (C_00844694* self, void* , int a2, int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, NetSession_GetEntryFlags, 0x0073CCE0, (int a1));
CSF_FUNC(int, __fastcall, GetSpawnFlagFromGlobal, 0x0073CD30, (void* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083e8c8_SelectHandleOrRequestSpawn(
    C_0083e8c8* self, void* /*edx*/, int handle)
{
    const int globalState = GetGlobalState_50D190();
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(globalState), 0xD0))
        return globalState;

    GetGlobal8B0688();
    const int gameMode = GetMpGameModeIndex();
    void* handleData = reinterpret_cast<void*>(handle);

    if (gameMode != 3)
    {
        int entryFlags = -1;
        if (CSF_AT(int, handleData, 0x08) >= 0)
        {
            const int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
            if (resolved)
            {
                const int entry = CSF_AT(int, reinterpret_cast<void*>(resolved), 0x18);
                GetGlobalDword_73C020();
                entryFlags = NetSession_GetEntryFlags(entry);
            }
        }
        self->field_0040 = entryFlags;
    }
    else if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                 CSF_AT(void**, self, 0)[0xB4 / 4])(self, nullptr))
    {
        self->field_0008 = 1;
        C_0083e8c8_PrepareSpawnSelection(self, nullptr, handle);

        if (CSF_AT(int, handleData, 0x08) < 0 ||
            CSF_AT(uint32_t, handleData, 0x04) ==
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 0x20))
        {
            const int session = GetGlobalDword_73C020();
            const int spawnFlag =
                GetSpawnFlagFromGlobal(reinterpret_cast<void*>(session), nullptr);
            auto* manager = reinterpret_cast<C_00844694*>(GetGlobal8B0688());
            return C_00844694_ForwardSpawnRequest(
                manager, nullptr, 1112354867, spawnFlag, -1, -1);
        }

        const int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
        int entryFlags = -1;
        if (resolved)
        {
            const int entry = CSF_AT(int, reinterpret_cast<void*>(resolved), 0x18);
            GetGlobalDword_73C020();
            entryFlags = NetSession_GetEntryFlags(entry);
        }
        self->field_0040 = entryFlags;

        const uint32_t targetEntry =
            CSF_AT(uint32_t, reinterpret_cast<void*>(resolved), 0x18);
        void* targetEntity = targetEntry
            ? reinterpret_cast<void*>(targetEntry - 1712)
            : nullptr;

        const uint32_t currentEntry =
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 0x18);
        void* currentEntity = currentEntry
            ? reinterpret_cast<void*>(currentEntry - 1712)
            : nullptr;

        if (CSF_AT(uint8_t, targetEntity, 0x443) ==
            CSF_AT(uint8_t, currentEntity, 0x443))
        {
            const int session = GetGlobalDword_73C020();
            const int spawnFlag =
                GetSpawnFlagFromGlobal(reinterpret_cast<void*>(session), nullptr);
            const int flags = static_cast<int>(self->field_0040);
            auto* manager = reinterpret_cast<C_00844694*>(GetGlobal8B0688());
            return C_00844694_ForwardSpawnRequest(
                manager, nullptr, 1112354868, flags, spawnFlag, -1);
        }
        else
        {
            const int session = GetGlobalDword_73C020();
            const int spawnFlag =
                GetSpawnFlagFromGlobal(reinterpret_cast<void*>(session), nullptr);
            const int flags = static_cast<int>(self->field_0040);
            auto* manager = reinterpret_cast<C_00844694*>(GetGlobal8B0688());
            return C_00844694_ForwardSpawnRequest(
                manager, nullptr, 1112354869, flags, spawnFlag, -1);
        }
    }

    C_0083e8c8_ApplyHandle(self, nullptr, handle);
    void* selectedObject = reinterpret_cast<void*>(self->field_000c);
    const int flags = static_cast<int>(self->field_0040);
    const int result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, selectedObject, 0)[0x5C / 4])(
            selectedObject, nullptr, flags);
    self->field_0040 = -1;
    return result;
}

HOOK(0x004E27D0, C_0083e8c8_SelectHandleOrRequestSpawn, "auto");
