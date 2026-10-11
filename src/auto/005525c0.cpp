#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MpSpawnPoints_ResetSelection, 0x00551440, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MpSpawnPoints_GetSpawnSelection, 0x00551BA0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(char*, __fastcall, MpSpawnPoints_GetPlayerSettings, 0x006B29A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(uint8_t, __fastcall, MenuMpPause_QueryGlobalFlagIsOne, 0x0073E040, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_SetSpawnIdentifiers, 0x007483E0,
         (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7));
CSF_FUNC(char, __fastcall, ConsumeByteFlag, 0x00748460, (void* self, void*));
CSF_FUNC(int64_t, __fastcall, MpSpawnPoints_GetSpawnIdentifier, 0x00748470,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetSettingsSelection, 0x00749920, (int a1, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(int, g_MpSessionState, 0x008B0690);
CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

// The packet also lists the reset routine's address as a vtable reference.
static const auto g_ResetSelectionVtableReference = CSF_VTBL(0x00551440);

static void __fastcall S_00551440_UpdateSpawnSelection(
    S_00551440* self, void* /*edx*/, float elapsedTime)
{
    if (!GetGlobalState_6A6D70())
        return;

    void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    if (!((bool(__fastcall*)(void*, void*))
          CSF_AT(void**, state, 0)[0x6C / 4])(state, nullptr))
        return;

    if (!g_CharacterSelectState8B0694)
        return;

    uint32_t selectedInterface = CSF_AT(
        uint32_t, reinterpret_cast<void*>(g_CharacterSelectState8B0694), 0x4);
    if (!selectedInterface)
        return;

    uint32_t playerAddress = selectedInterface - 776;
    if (!playerAddress)
        return;

    void* player = reinterpret_cast<void*>(playerAddress);
    void* selectionInterface = reinterpret_cast<uint8_t*>(player) + 0x308;

    if (!((bool(__fastcall*)(void*, void*))
          CSF_AT(void**, selectionInterface, 0)[0x8 / 4])(
              selectionInterface, nullptr))
    {
        if (ConsumeByteFlag(player, nullptr))
        {
            void* globalState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            void** stateVtable = CSF_AT(void**, globalState, 0);
            int64_t identifier2 = MpSpawnPoints_GetSpawnIdentifier(player, nullptr, 2);
            int64_t identifier1 = MpSpawnPoints_GetSpawnIdentifier(player, nullptr, 1);
            int64_t identifier0 = MpSpawnPoints_GetSpawnIdentifier(player, nullptr, 0);

            ((void(__fastcall*)(void*, void*, int64_t, int64_t, int64_t))
             stateVtable[0x80 / 4])(
                 globalState, nullptr, identifier0, identifier1, identifier2);
        }
        return;
    }

    bool initialRequest = false;
    switch (self->field_1444)
    {
    case 0:
        if (CSF_AT(uint8_t, self, 0x1480))
            return;
        if (!((bool(__fastcall*)(void*, void*))
              CSF_AT(void**, selectionInterface, 0)[0x8 / 4])(
                  selectionInterface, nullptr))
            return;

        CSF_AT(uint8_t, self, 0x1480) = 1;
        MpSpawnPoints_ResetSelection(self, nullptr);
        self->field_1444 = 1;
        initialRequest = true;
        break;

    case 1:
    {
        void* globalState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        int mode = ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, globalState, 0)[0x1C / 4])(
                        globalState, nullptr);
        if (mode == 3)
            return;

        if (mode == 7)
        {
            void* state2 = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            void* state1 = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            void* state0 = reinterpret_cast<void*>(GetGlobalState_6A6D70());

            int64_t identifier2 =
                ((int64_t(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, state2, 0)[0x7C / 4])(state2, nullptr, 2);
            int64_t identifier1 =
                ((int64_t(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, state1, 0)[0x7C / 4])(state1, nullptr, 1);
            int64_t identifier0 =
                ((int64_t(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, state0, 0)[0x7C / 4])(state0, nullptr, 0);

            MpSpawnPoints_SetSpawnIdentifiers(
                player, nullptr,
                static_cast<int32_t>(identifier0),
                static_cast<int32_t>(identifier0 >> 32),
                static_cast<int32_t>(identifier1),
                static_cast<int32_t>(identifier1 >> 32),
                static_cast<int32_t>(identifier2),
                static_cast<int32_t>(identifier2 >> 32));
            self->field_1444 = 2;
            self->field_144c = 0.0f;
            return;
        }

        if (CSF_AT(int32_t, self, 0x1454) > 3)
        {
            self->field_1444 = 0;
            g_MpSessionState = 1;
            return;
        }

        self->field_1444 = 1;
        break;
    }

    case 2:
    {
        float remainingTime = self->field_144c - elapsedTime;
        self->field_144c = remainingTime;
        if (remainingTime < g_fZero)
        {
            self->field_1460 = 1;
            self->field_1444 = 0;
        }
        return;
    }

    default:
        return;
    }

    void* requestState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    char* nameSettings = MpSpawnPoints_GetPlayerSettings(player, nullptr);
    char* secondarySettings = MpSpawnPoints_GetPlayerSettings(player, nullptr);
    char* flagSettings = MpSpawnPoints_GetPlayerSettings(player, nullptr);
    uint8_t setting3 = CSF_AT(
        uint8_t, MpSpawnPoints_GetPlayerSettings(player, nullptr), 0x3);
    uint8_t setting5 = CSF_AT(
        uint8_t, MpSpawnPoints_GetPlayerSettings(player, nullptr), 0x5);
    int setting6 = CSF_AT(
        int8_t, MpSpawnPoints_GetPlayerSettings(player, nullptr), 0x6);

    void** requestVtable = CSF_AT(void**, requestState, 0);
    int spawnSelection = MpSpawnPoints_GetSpawnSelection();
    uint8_t hasSecondarySetting = CSF_AT(uint8_t, flagSettings, 0x107) != 0;

    char* selectionSettings = MpSpawnPoints_GetPlayerSettings(player, nullptr);
    int settingsSelection = MpSpawnPoints_GetSettingsSelection(
        reinterpret_cast<int>(selectionSettings), 0);
    void* pauseState = reinterpret_cast<void*>(GetGlobalDword_73C020());
    int pauseFlag = static_cast<int8_t>(
        MenuMpPause_QueryGlobalFlagIsOne(pauseState, nullptr));

    ((void(__fastcall*)(void*, void*, int, int, int, uint8_t, uint8_t,
                       uint8_t, char*, char*, int))
     requestVtable[0x70 / 4])(
         requestState, nullptr, pauseFlag, settingsSelection, setting6,
         setting5, setting3, hasSecondarySetting,
         secondarySettings + 263, nameSettings + 7, spawnSelection);

    if (initialRequest)
        CSF_AT(uint32_t, self, 0x1454) = 0;
    else
        ++CSF_AT(uint32_t, self, 0x1454);
}

HOOK(0x005525C0, S_00551440_UpdateSpawnSelection, "auto");
