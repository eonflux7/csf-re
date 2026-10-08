#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_GLOBAL(std::uint8_t, g_AccountMenuStateFlag, 0x008B0901);
static CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static CSF_FUNC(void*, __fastcall, MenuMpAccounts_FindAccountByName,
                0x0055BCB0, (void* self, void* edx, char* name));
static CSF_FUNC(int, __fastcall, MenuAccounts_CopyWideNameToBuffer,
                0x0055D430, (void* self, void* edx, std::uint16_t* destination));
static CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey,
                0x00692440, (int identifier, int mode));
static CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow,
                0x00695620, (std::uint16_t* source, char* destination,
                             unsigned int capacity));
static CSF_FUNC(void*, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());

static int CMenuPlayerInfo_SetControlFlag(void* control, int flag)
{
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, flag);
}

static void* CMenuPlayerInfo_GetControlText(void* control)
{
    return reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[0x110 / 4])(control, nullptr);
}

static std::uint8_t __fastcall CMenuPlayerInfo_UpdateFriendshipControls(
    CMenuPlayerInfo* self, void* /*edx*/)
{
    std::uint8_t active = g_AccountMenuStateFlag;
    if (active)
        active = g_OnlineMatchFlag;

    if (!active)
    {
        if (self->field_0040 == 1)
            return active;

        CMenuPlayerInfo_SetControlFlag(
            reinterpret_cast<void*>(self->field_0020), 1);
        const int result = CMenuPlayerInfo_SetControlFlag(
            reinterpret_cast<void*>(self->field_0034), 1);
        self->field_0040 = 1;
        return static_cast<std::uint8_t>(result);
    }

    std::uint16_t wideName[256];
    char name[256] = {};

    MenuAccounts_CopyWideNameToBuffer(
        reinterpret_cast<void*>(self->field_0048), nullptr, wideName);
    ConvertWideTextToNarrow(wideName, name, 256);

    void* globalState = GetGlobalState_6A6D70();
    void* accounts = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, globalState, 0)[0xF4 / 4])(globalState, nullptr);
    void* account = MenuMpAccounts_FindAccountByName(accounts, nullptr, name);

    std::uint32_t nextState;
    int textIdentifier;
    bool useInfoControl;
    std::uint32_t status = 0;

    if (account)
    {
        status = CSF_AT(std::uint32_t, account, 0x90);
        switch (status)
        {
        case 0:
            nextState = 4;
            textIdentifier = 0x46523332;
            useInfoControl = true;
            break;
        case 1:
            nextState = 2;
            textIdentifier = 0x46523931;
            useInfoControl = false;
            break;
        case 2:
            nextState = 5;
            textIdentifier = 0x46523131;
            useInfoControl = true;
            break;
        case 3:
            nextState = 2;
            textIdentifier = 0x4F505439;
            useInfoControl = false;
            break;
        default:
            return static_cast<std::uint8_t>(status);
        }
    }
    else
    {
        nextState = 6;
        textIdentifier = 0x4F505439;
        useInfoControl = false;
    }

    if (self->field_0040 == nextState)
        return static_cast<std::uint8_t>(status);

    CMenuPlayerInfo_SetControlFlag(
        reinterpret_cast<void*>(self->field_0020), useInfoControl ? 1 : 0);
    CMenuPlayerInfo_SetControlFlag(
        reinterpret_cast<void*>(self->field_0034), useInfoControl ? 0 : 1);

    void* textControl = CMenuPlayerInfo_GetControlText(
        reinterpret_cast<void*>(
            useInfoControl ? self->field_0034 : self->field_0020));

    // The original saves this vtable before the localization call.
    void** textVtable = CSF_AT(void**, textControl, 0);
    const int localizedText =
        MenuMessageBox_ResolveEntryByKey(textIdentifier, 1);
    const int result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        textVtable[0x40 / 4])(textControl, nullptr, localizedText);

    self->field_0040 = nextState;
    return static_cast<std::uint8_t>(result);
}

HOOK(0x0055D4C0, CMenuPlayerInfo_UpdateFriendshipControls, "auto");
