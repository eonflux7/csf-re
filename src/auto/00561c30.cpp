#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(char*, __cdecl, GetSelectedCharacterSpawnName, 0x00551310, ());
CSF_FUNC(int, __fastcall, CMenuMpSessions_RebuildPartidasList, 0x00560890, (CMenuMpSessions* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));
CSF_FUNC(int*, __fastcall, DetachAndForwardLinkedNode, 0x007223B0, (void* self, void* , int16_t* a2));

static void MenuMpSessions_SetControlValue(void* control, int value)
{
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, control, 0)[672 / 4])(
        control, nullptr, value);
}

static void MenuMpSessions_SelectControlEntry(void* control, int entry)
{
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, control, 0)[676 / 4])(
        control, nullptr, entry);
}

static void MenuMpSessions_SetControlRange(void* control, int maximum, int minimum)
{
    ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, control, 0)[668 / 4])(
        control, nullptr, maximum, minimum);
}

static void MenuMpSessions_SetControlStep(void* control, int step)
{
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, control, 0)[680 / 4])(
        control, nullptr, step);
}

static void* __fastcall CMenuMpSessions_LoadSelectedSessionSettings(
    CMenuMpSessions* self, void* /*edx*/)
{
    alignas(8) uint8_t sessionNameBytes[0x1C];
    alignas(8) uint8_t passwordBytes[0x1C];

    GetGlobal8B0688();
    char* settings = GetSelectedCharacterSpawnName();

    FormatterSetPattern(sessionNameBytes, nullptr, settings + 135);
    int16_t* sessionName = CSF_AT(int16_t*, sessionNameBytes, 0x4);
    if (CSF_AT(uint32_t, sessionNameBytes, 0x18) < 8)
        sessionName = reinterpret_cast<int16_t*>(sessionNameBytes + 0x4);
    DetachAndForwardLinkedNode(
        reinterpret_cast<void*>(self->field_003c), nullptr, sessionName);

    self->field_0018 = CSF_AT(int8_t, settings, 0x6);
    MenuMpSessions_SetControlValue(
        reinterpret_cast<void*>(self->field_007c),
        CSF_AT(int8_t, settings, 0x5));
    MenuMpSessions_SelectControlEntry(
        reinterpret_cast<void*>(self->field_0078),
        1668100913 - (CSF_AT(uint8_t, settings, 0x3) != 0));

    self->field_0200 = CSF_AT(uint8_t, settings, 0x107) != 0;
    FormatterSetPattern(passwordBytes, nullptr, settings + 263);
    void* passwordEdit = reinterpret_cast<void*>(self->field_01fc);
    if (passwordEdit)
    {
        int16_t* password = CSF_AT(int16_t*, passwordBytes, 0x4);
        if (CSF_AT(uint32_t, passwordBytes, 0x18) < 8)
            password = reinterpret_cast<int16_t*>(passwordBytes + 0x4);
        DetachAndForwardLinkedNode(passwordEdit, nullptr, password);
    }

    MenuMpSessions_SelectControlEntry(
        reinterpret_cast<void*>(self->field_0024),
        1668100913 - (CSF_AT(uint8_t, settings, 0x0) != 0));
    MenuMpSessions_SelectControlEntry(
        reinterpret_cast<void*>(self->field_0028),
        1668100913 - (CSF_AT(uint8_t, settings, 0x1) != 0));
    MenuMpSessions_SelectControlEntry(
        reinterpret_cast<void*>(self->field_002c),
        1668100913 - (CSF_AT(uint8_t, settings, 0x2) != 0));

    const uint32_t gameMode = self->field_0018;
    if (gameMode == 0 || gameMode == 1 || gameMode == 3)
    {
        int scoreMaximum;
        int scoreStep;
        switch (gameMode)
        {
        case 0:
            MenuMpSessions_SelectControlEntry(
                reinterpret_cast<void*>(self->field_0074), 1668100145);
            scoreMaximum = 200;
            scoreStep = 10;
            break;
        case 1:
            MenuMpSessions_SelectControlEntry(
                reinterpret_cast<void*>(self->field_0074), 1668100146);
            scoreMaximum = 200;
            scoreStep = 10;
            break;
        default: // Mode 3.
            MenuMpSessions_SelectControlEntry(
                reinterpret_cast<void*>(self->field_0074), 1668100148);
            scoreMaximum = 10;
            scoreStep = 1;
            break;
        }

        void** scoreVtable = CSF_AT(
            void**, reinterpret_cast<void*>(self->field_0084), 0);
        int scoreEntry = MenuMessageBox_ResolveEntryByKey(1129132596, 1);
        ((void(__fastcall*)(void*, void*, int, int))scoreVtable[664 / 4])(
            reinterpret_cast<void*>(self->field_0084), nullptr, 0, scoreEntry);

        MenuMpSessions_SetControlRange(
            reinterpret_cast<void*>(self->field_0084), scoreMaximum, 0);
        MenuMpSessions_SetControlStep(
            reinterpret_cast<void*>(self->field_0084), scoreStep);

        switch (gameMode)
        {
        case 0:
            MenuMpSessions_SetControlValue(
                reinterpret_cast<void*>(self->field_0084),
                CSF_AT(uint16_t, settings, 0x196));
            break;
        case 1:
            MenuMpSessions_SetControlValue(
                reinterpret_cast<void*>(self->field_0084),
                CSF_AT(uint16_t, settings, 0x198));
            break;
        default:
            MenuMpSessions_SetControlValue(
                reinterpret_cast<void*>(self->field_0084),
                CSF_AT(uint16_t, settings, 0x19C));
            break;
        }

        void** timeVtable = CSF_AT(
            void**, reinterpret_cast<void*>(self->field_0080), 0);
        int timeEntry = MenuMessageBox_ResolveEntryByKey(1129132596, 1);
        ((void(__fastcall*)(void*, void*, int, int))timeVtable[664 / 4])(
            reinterpret_cast<void*>(self->field_0080), nullptr, 0, timeEntry);

        MenuMpSessions_SetControlRange(
            reinterpret_cast<void*>(self->field_0080), 60, 0);
        MenuMpSessions_SetControlStep(
            reinterpret_cast<void*>(self->field_0080), 5);

        int64_t timeProduct;
        switch (gameMode)
        {
        case 0:
            timeProduct = 1172812403LL * CSF_AT(uint32_t, settings, 0x188);
            break;
        case 1:
            timeProduct = 1172812403LL * CSF_AT(uint32_t, settings, 0x190);
            break;
        default:
            timeProduct = 1172812403LL * CSF_AT(uint32_t, settings, 0x18C);
            break;
        }
        const uint32_t timeValue =
            static_cast<uint32_t>(timeProduct >> 32) >> 14;
        MenuMpSessions_SetControlValue(
            reinterpret_cast<void*>(self->field_0080),
            static_cast<int>(timeValue));
    }

    CMenuMpSessions_RebuildPartidasList(self, nullptr);

    if (CSF_AT(uint32_t, passwordBytes, 0x18) >= 8 &&
        CSF_AT(void*, passwordBytes, 0x4))
    {
        Mem_FreeDebug(CSF_AT(void*, passwordBytes, 0x4));
    }

    void* result = reinterpret_cast<void*>(
        CSF_AT(uint32_t, sessionNameBytes, 0x18));
    CSF_AT(uint32_t, passwordBytes, 0x18) = 7;
    CSF_AT(uint32_t, passwordBytes, 0x14) = 0;
    CSF_AT(uint16_t, passwordBytes, 0x4) = 0;

    if (CSF_AT(uint32_t, sessionNameBytes, 0x18) >= 8)
    {
        result = CSF_AT(void*, sessionNameBytes, 0x4);
        if (result)
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
    }
    return result;
}

HOOK(0x00561C30, CMenuMpSessions_LoadSelectedSessionSettings, "auto");
