#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalDword_8B0680, 0x0054E520, ());
CSF_FUNC(uint8_t*, __fastcall, GetSessionEntryByIndex, 0x0054E610, (void* self, void* edx, int index));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __fastcall, CMenuMpSessions_RebuildPartidasList, 0x00560890, (CMenuMpSessions* self, void*));
CSF_FUNC(void, __fastcall, CMenuMpSessions_ConfigureSession, 0x00562420, (void* self, void* edx, char a2, char* a3, int a4, int a5, int a6, int a7, char* a8, int a9, char* Source, char a11, char a12, char a13, int16_t a14, int a15));
CSF_FUNC(int, __fastcall, GetIndexedTableValue_714C20, 0x00714C20, (int* self, void* , int rowIndex, int columnIndex));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static int __fastcall CMenuMpSessions_InitializeSession(
    CMenuMpSessions* self, void* /*edx*/)
{
    alignas(8) std::uint8_t sessionNameBytes[0x104];
    alignas(8) std::uint8_t passwordBytes[0x100];
    char* sessionName = reinterpret_cast<char*>(sessionNameBytes);
    char* password = reinterpret_cast<char*>(passwordBytes);

    // The original copies the default name from these five constant chunks.
    std::memcpy(sessionName, CSF_STR(0x00845A80, "AutoSessionCSF"), 4);
    std::memcpy(sessionName + 4, CSF_STR(0x00845A84, "SessionCSF"), 4);
    std::memcpy(sessionName + 8, CSF_STR(0x00845A88, "ionCSF"), 4);
    std::memcpy(sessionName + 12, CSF_STR(0x00845A8C, "SF"), 2);
    std::memcpy(sessionName + 14, CSF_STR(0x00845A8E, ""), 1);
    std::memset(sessionName + 15, 0, 241);
    std::memset(passwordBytes, 0, sizeof(passwordBytes));

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    std::uint32_t stateValue = GetStateValue(state, nullptr, 2);
    self->field_0018 = stateValue;

    std::int16_t sessionLimit = 0;
    int duration = 0;
    if (stateValue == 0)
    {
        sessionLimit = 20;
        duration = 600000;
    }
    else if (stateValue - 1 <= 1)
    {
        sessionLimit = 5;
        duration = 1200000;
        self->field_0018 = 3;
    }
    CSF_AT(int, sessionNameBytes, 0x100) = duration;

    CMenuMpSessions_RebuildPartidasList(self, nullptr);
    int result = static_cast<int>(self->field_01f4);
    if (result > 0)
    {
        const int selectedIndex = result;
        state = reinterpret_cast<void*>(GetGlobalState_50D190());
        const int rowIndex = GetStateValue(state, nullptr, selectedIndex);
        const int entryIndex = GetIndexedTableValue_714C20(
            reinterpret_cast<int*>(self->field_0034), nullptr, rowIndex, 0);

        void* sessions = reinterpret_cast<void*>(GetGlobalDword_8B0680());
        std::uint8_t* entry =
            GetSessionEntryByIndex(sessions, nullptr, entryIndex);

        char onlineMatch;
        if (GetGlobal8B0688())
        {
            GetGlobal8B0688();
            onlineMatch = static_cast<char>(g_OnlineMatchFlag);
        }
        else
        {
            onlineMatch = g_MpNetworkType == 3;
        }

        CMenuMpSessions_ConfigureSession(
            self, nullptr, onlineMatch, sessionName,
            static_cast<int>(self->field_0018), 16, 0, 0, password,
            reinterpret_cast<int>(entry + 256),
            reinterpret_cast<char*>(entry),
            1, 1, 1, sessionLimit, duration);

        result = GetGlobalDword_73C020();
        if (CSF_AT(int, reinterpret_cast<void*>(result), 0x98) == 6)
        {
            self->field_01e4 = 3;
            return result;
        }
    }

    self->field_01f1 = 1;
    return result;
}

HOOK(0x00563930, CMenuMpSessions_InitializeSession, "auto");
