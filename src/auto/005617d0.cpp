#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __cdecl, GetGlobalDword_8B0680, 0x0054E520, ());
CSF_FUNC(uint8_t*, __fastcall, GetSessionEntryByIndex, 0x0054E610, (void* self, void* edx, int index));
CSF_FUNC(int, __fastcall, GetIndexedTableValue_714C20, 0x00714C20, (int* self, void* , int rowIndex, int columnIndex));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex, 0x00714CA0, (void* self, void* , int index));

static uint32_t __fastcall CMenuMpSessionsPC_SubmitSessionSettings(
    CMenuMpSessionsPC* self, void* /*edx*/)
{
    void* manager = CSF_AT(void*,
        reinterpret_cast<void*>(GetGlobalValue_8B056C()), 0x14);
    const uint32_t sessionId = CSF_AT(uint32_t, manager, 0x04);
    if (sessionId == static_cast<uint32_t>(-1))
        return sessionId;

    void* settings =
        (reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, manager, 0)[0x30 / 4]))(
                manager, nullptr, sessionId);
    if (!settings)
        return 0;

    uint8_t* sessionEntry = nullptr;
    if (self->field_0034)
    {
        const int rowIndex = GetStoredValueAtIndex(
            reinterpret_cast<void*>(self->field_0034), nullptr, 0);
        const int entryIndex = GetIndexedTableValue_714C20(
            reinterpret_cast<int*>(self->field_0034), nullptr, rowIndex, 0);
        void* sessionDirectory =
            reinterpret_cast<void*>(GetGlobalDword_8B0680());
        sessionEntry = GetSessionEntryByIndex(
            sessionDirectory, nullptr, entryIndex);
    }

    CSF_AT(uint32_t, settings, 0x40) = self->field_0018;
    CSF_AT(uint32_t, settings, 0x44) = self->field_001c;

    if (sessionEntry)
    {
        uint8_t* destination = reinterpret_cast<uint8_t*>(settings) + 0x48;
        uint8_t character;
        do
        {
            character = *sessionEntry++;
            *destination++ = character;
        } while (character);
    }
    else
    {
        CSF_AT(uint8_t, settings, 0x48) = 0;
    }

    void* control = reinterpret_cast<void*>(self->field_0024);
    CSF_AT(uint8_t, settings, 0x249) =
        control && !(reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x2AC / 4]))(control, nullptr);

    control = reinterpret_cast<void*>(self->field_0028);
    CSF_AT(uint8_t, settings, 0x24A) =
        control && !(reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x2AC / 4]))(control, nullptr);

    control = reinterpret_cast<void*>(self->field_002c);
    CSF_AT(uint8_t, settings, 0x24B) =
        control && !(reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x2AC / 4]))(control, nullptr);

    SceneLoader_NoOp();

    const bool submitted =
        (reinterpret_cast<bool (__fastcall*)(void*, void*, uint32_t, void*)>(
            CSF_AT(void**, manager, 0)[0x1C / 4]))(
                manager, nullptr, sessionId, settings);

    if (submitted)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_DispatchMenuRequest(state, nullptr, 1095053623, 1, 1, 0);
        self->field_0014 = 0;
        return (reinterpret_cast<uint32_t (__fastcall*)(void*, void*)>(
            CSF_AT(void**, manager, 0)[0x28 / 4]))(manager, nullptr);
    }

    void* currentManager = CSF_AT(void*,
        reinterpret_cast<void*>(GetGlobalValue_8B056C()), 0x14);
    const int errorEntry =
        (reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, currentManager, 0)[0x54 / 4]))(
                currentManager, nullptr);
    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    const int result = GameBddLoader_DispatchMenuRequest(
        state, nullptr, errorEntry, 1, 1, 0);
    self->field_0014 = 0;
    return static_cast<uint32_t>(result);
}

HOOK(0x005617D0, CMenuMpSessionsPC_SubmitSessionSettings, "auto");
