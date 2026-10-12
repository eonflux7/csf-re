#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __cdecl, GetGlobalDword_8B0680, 0x0054E520, ());
CSF_FUNC(uint8_t*, __fastcall, GetSessionEntryByIndex, 0x0054E610, (void* self, void* edx, int index));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(void, __fastcall, C_00844694_ApplyCharacterSelection, 0x00553E20, (C_00844694* self, void* , char selectionType, char* primaryString, char option4, char option5, char selectionMode, int , char* tertiaryString, int selectionParameter, char* secondaryString, char option11, char option12, char option13, int16_t shortValue, int value));
CSF_FUNC(void, __fastcall, CMenuMpSessions_CreateSession, 0x00562420, (void* self, void* edx, char a2, char* a3, int a4, int a5, int a6, int a7, char* a8, int a9, char* Source, char a11, char a12, char a13, int16_t a14, int a15));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620, (std::uint16_t* source, char* destination, unsigned int length));
CSF_FUNC(int, __fastcall, MenuAccount_GateCheck_696B60, 0x00696B60, (void* self, void* edx, void* a2));
CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(int, __fastcall, GetIndexedTableValue_714C20, 0x00714C20, (int* self, void* , int rowIndex, int columnIndex));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex, 0x00714CA0, (void* self, void* , int index));
CSF_FUNC(void*, __fastcall, GetContainerDataPtr, 0x00721270, (void* self, void*));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static void* __fastcall CMenuMpSessions_BuildSessionEntry(
    CMenuMpSessions* self, void* /*edx*/)
{
    alignas(8) uint8_t passwordBytes[0x1C];
    CSF_AT(uint32_t, passwordBytes, 0x18) = 7;
    CSF_AT(uint32_t, passwordBytes, 0x14) = 0;
    CSF_AT(uint16_t, passwordBytes, 0x04) = 0;

    void* control = reinterpret_cast<void*>(self->field_01fc);
    if (control) {
        auto* text = static_cast<int16_t*>(GetContainerDataPtr(control, nullptr));
        MenuProgressInput_AssignText(
            reinterpret_cast<uint32_t*>(passwordBytes), nullptr, text);
    }

    alignas(8) char password[0x100];
    std::memset(password, 0, sizeof(password));
    auto* passwordData = CSF_AT(uint32_t, passwordBytes, 0x18) < 8
        ? reinterpret_cast<uint16_t*>(passwordBytes + 0x04)
        : CSF_AT(uint16_t*, passwordBytes, 0x04);
    ConvertWideTextToNarrow(passwordData, password, 256);

    control = reinterpret_cast<void*>(self->field_0024);
    const char option11 = control &&
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, control, 0)[684 / 4])
            (control, nullptr) == 0;
    control = reinterpret_cast<void*>(self->field_0028);
    const char option12 = control &&
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, control, 0)[684 / 4])
            (control, nullptr) == 0;
    control = reinterpret_cast<void*>(self->field_002c);
    const char option13 = control &&
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, control, 0)[684 / 4])
            (control, nullptr) == 0;
    control = reinterpret_cast<void*>(self->field_0030);
    self->field_0200 = control &&
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, control, 0)[684 / 4])
            (control, nullptr) == 0;

    void* result;
    if (!option11 && !option12 && !option13) {
        SceneLoader_NoOp();
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        result = reinterpret_cast<void*>(
            GameBddLoader_DispatchMenuRequest(state, nullptr, 1129132851, 1, 1, 0));
        self->field_0014 = 0;
    } else if (self->field_0018 == 3 && !option12) {
        SceneLoader_NoOp();
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        result = reinterpret_cast<void*>(
            GameBddLoader_DispatchMenuRequest(state, nullptr, 1129132852, 1, 1, 0));
        self->field_0014 = 0;
    } else {
        alignas(8) uint8_t secondPasswordBytes[0x1C];
        CSF_AT(uint32_t, secondPasswordBytes, 0x18) = 7;
        CSF_AT(uint32_t, secondPasswordBytes, 0x14) = 0;
        CSF_AT(uint16_t, secondPasswordBytes, 0x04) = 0;
        control = reinterpret_cast<void*>(self->field_01fc);
        if (control) {
            auto* text = static_cast<int16_t*>(GetContainerDataPtr(control, nullptr));
            MenuProgressInput_AssignText(
                reinterpret_cast<uint32_t*>(secondPasswordBytes), nullptr, text);
        }

        alignas(8) uint8_t sessionNameBytes[0x3C];
        S_00696af0& sessionName =
            *reinterpret_cast<S_00696af0*>(sessionNameBytes);
        auto* nameText = static_cast<wchar_t*>(GetContainerDataPtr(
            reinterpret_cast<void*>(self->field_003c), nullptr));
        ConstructWideString(&sessionName, nullptr, nameText);
        bool emptyName = static_cast<uint8_t>(MenuAccount_GateCheck_696B60(
            &sessionName, nullptr, &g_byte_82E65C)) != 0;
        if (!emptyName) {
            emptyName = true;
            for (uint32_t index = 0;
                 index < CSF_AT(uint32_t, &sessionName, 0x14); ++index) {
                if (!emptyName)
                    break;
                auto* data = sessionName.field_0018 < 8
                    ? reinterpret_cast<uint16_t*>(&sessionName.field_0004)
                    : reinterpret_cast<uint16_t*>(sessionName.field_0004);
                if (data[index] != 32)
                    emptyName = false;
            }
        }

        alignas(8) char sessionNameNarrow[0x100];
        std::memset(sessionNameNarrow, 0, sizeof(sessionNameNarrow));
        auto* nameData = sessionName.field_0018 < 8
            ? reinterpret_cast<uint16_t*>(&sessionName.field_0004)
            : reinterpret_cast<uint16_t*>(sessionName.field_0004);
        ConvertWideTextToNarrow(nameData, sessionNameNarrow, 256);

        if (emptyName) {
            SceneLoader_NoOp();
            void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
            GameBddLoader_DispatchMenuRequest(state, nullptr, 1129132597, 1, 1, 0);
            self->field_0014 = 0;
        } else {
            int row = GetStoredValueAtIndex(
                reinterpret_cast<void*>(self->field_0034), nullptr, 0);
            int entryIndex = GetIndexedTableValue_714C20(
                reinterpret_cast<int*>(self->field_0034), nullptr, row, 0);
            void* sessions = reinterpret_cast<void*>(GetGlobalDword_8B0680());
            char* entry = reinterpret_cast<char*>(
                GetSessionEntryByIndex(sessions, nullptr, entryIndex));

            if (self->field_01e0 == 3) {
                GetGlobal8B0688();
                uint8_t online = g_OnlineMatchFlag;
                void* modeControl = reinterpret_cast<void*>(self->field_0078);
                void* timeControl = reinterpret_cast<void*>(self->field_0080);
                void* limitControl = reinterpret_cast<void*>(self->field_0084);
                void* optionControl = reinterpret_cast<void*>(self->field_007c);

                int timeValue =
                    ((int(__fastcall*)(void*, void*))CSF_AT(void**, timeControl, 0)[676 / 4])
                        (timeControl, nullptr);
                int duration = static_cast<int>(60000u * static_cast<uint32_t>(timeValue));
                int16_t limit = static_cast<int16_t>(
                    ((int(__fastcall*)(void*, void*))CSF_AT(void**, limitControl, 0)[676 / 4])
                        (limitControl, nullptr));
                int privateGame = self->field_0200;
                char mode =
                    ((int(__fastcall*)(void*, void*))CSF_AT(void**, modeControl, 0)[684 / 4])
                        (modeControl, nullptr) == 0;
                char option = static_cast<char>(
                    ((int(__fastcall*)(void*, void*))CSF_AT(void**, optionControl, 0)[676 / 4])
                        (optionControl, nullptr));
                char sessionType = static_cast<char>(self->field_0018);
                char selectionType = online == 0;
                auto* manager = reinterpret_cast<C_00844694*>(GetGlobal8B0688());
                C_00844694_ApplyCharacterSelection(
                    manager, nullptr, selectionType, sessionNameNarrow,
                    sessionType, option, mode, privateGame, password,
                    reinterpret_cast<int>(entry + 256), entry,
                    option11, option12, option13, limit, duration);
            } else {
                char* privateMarker = self->field_0200
                    ? reinterpret_cast<char*>(CSF_ADDR_DATA(0x00845A90))
                    : reinterpret_cast<char*>(&g_byte_82E65C);
                char selectionType;
                if (GetGlobal8B0688()) {
                    GetGlobal8B0688();
                    selectionType = g_OnlineMatchFlag == 0;
                } else {
                    selectionType = g_MpNetworkType == 2;
                }

                void* timeControl = reinterpret_cast<void*>(self->field_0080);
                void* optionControl = reinterpret_cast<void*>(self->field_007c);
                void* limitControl = reinterpret_cast<void*>(self->field_0084);
                void* modeControl = reinterpret_cast<void*>(self->field_0078);
                int timeValue =
                    ((int(__fastcall*)(void*, void*))CSF_AT(void**, timeControl, 0)[676 / 4])
                        (timeControl, nullptr);
                int duration = static_cast<int>(60000u * static_cast<uint32_t>(timeValue));
                int16_t limit = static_cast<int16_t>(
                    ((int(__fastcall*)(void*, void*))CSF_AT(void**, limitControl, 0)[676 / 4])
                        (limitControl, nullptr));
                int privateGame = self->field_0200;
                int modeValue =
                    ((int(__fastcall*)(void*, void*))CSF_AT(void**, modeControl, 0)[684 / 4])
                        (modeControl, nullptr);
                uint32_t negatedMode = 0u - static_cast<uint32_t>(modeValue);
                int mode = static_cast<int>(
                    (negatedMode & ~0xFFu) | static_cast<uint32_t>(negatedMode == 0));
                int option =
                    ((int(__fastcall*)(void*, void*))CSF_AT(void**, optionControl, 0)[676 / 4])
                        (optionControl, nullptr);
                CMenuMpSessions_CreateSession(
                    self, nullptr, selectionType, sessionNameNarrow,
                    self->field_0018, option, mode, privateGame, privateMarker,
                    reinterpret_cast<int>(entry + 256), entry,
                    option11, option12, option13, limit, duration);
            }
        }

        if (sessionName.field_0018 >= 8 && sessionName.field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(sessionName.field_0004));
        result = nullptr;
        CSF_AT(uint32_t, &sessionName, 0x14) = 0;
        CSF_AT(uint16_t, &sessionName.field_0004, 0) = 0;
        sessionName.field_0018 = 7;
        if (CSF_AT(uint32_t, secondPasswordBytes, 0x18) >= 8) {
            result = CSF_AT(void*, secondPasswordBytes, 0x04);
            if (result)
                result = reinterpret_cast<void*>(Mem_FreeDebug(result));
        }
    }

    if (CSF_AT(uint32_t, passwordBytes, 0x18) >= 8) {
        result = CSF_AT(void*, passwordBytes, 0x04);
        if (result)
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
    }
    return result;
}

HOOK(0x00563AB0, CMenuMpSessions_BuildSessionEntry, "auto");
