#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_GLOBAL(std::uint8_t, g_AccountMenuStateFlag, 0x008B0901);
static CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static CSF_FUNC(void*, __cdecl, GetGlobalValue, 0x00521BB0, ());
static CSF_FUNC(void, __fastcall, MenuMpAccounts_PrepareGlobalMenu,
                0x00521DE0, (void* self, void* edx));
static CSF_FUNC(int, __fastcall, CMenuPlayerInfo_RefreshScreen,
                0x0053AD90, (CMenuPlayerInfo* self, void* edx));
static CSF_FUNC(std::uint8_t, __fastcall, CMenuPlayerInfo_UpdateFriendshipControls,
                0x0055D4C0, (CMenuPlayerInfo* self, void* edx));
static CSF_FUNC(int, __fastcall, CMenuPlayerInfo_SetFriendRequestTextByState,
                0x0055D880, (CMenuPlayerInfo* self, void* edx));
static CSF_FUNC(int, __cdecl, MenuMpAccounts_GetWideStringLength,
                0x00695560, (std::uint16_t* text));
static CSF_FUNC(std::int16_t, __cdecl, MenuMpAccounts_CopyWideString,
                0x00695680, (char* destination, char* source));
static CSF_FUNC(void*, __cdecl, GetGlobalState, 0x006A6D70, ());
static CSF_FUNC(int, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16,
                0x00720DF0, (void* self, void* edx, std::uint16_t* text));

static void* CMenuPlayerInfo_FindControl(void* root, const char* name)
{
    return reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, root, 0)[0xFC / 4])(root, nullptr, name);
}

static int __fastcall CMenuPlayerInfo_InitializePlayerControls(
    CMenuPlayerInfo* self, void* /*edx*/,
    std::uint32_t context, std::uint32_t /*unused*/)
{
    std::uint16_t playerName[256];
    void** initialVtable = CSF_AT(void**, self, 0);

    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0038 = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0040 = 0;
    self->field_0044 = 0x40400000;

    reinterpret_cast<void(__fastcall*)(CMenuPlayerInfo*, void*)>(
        initialVtable[0x40 / 4])(self, nullptr);

    self->field_0014 = context;
    MenuMpAccounts_PrepareGlobalMenu(GetGlobalValue(), nullptr);

    void* root = reinterpret_cast<void*>(self->field_001c);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, root, 0)[0x16C / 4])(root, nullptr);

    void* control = CMenuPlayerInfo_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00845610, "EstaticoInfoAmistad"));
    root = reinterpret_cast<void*>(self->field_001c);
    self->field_0034 = reinterpret_cast<std::uint32_t>(control);

    control = CMenuPlayerInfo_FindControl(
        root, CSF_STR(0x00845508, "BotonRequestFriend"));
    self->field_0020 = reinterpret_cast<std::uint32_t>(control);
    if (control)
        CMenuPlayerInfo_UpdateFriendshipControls(self, nullptr);

    control = CMenuPlayerInfo_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x008455FC, "BotonToggleVoice"));
    root = reinterpret_cast<void*>(self->field_001c);
    self->field_0024 = reinterpret_cast<std::uint32_t>(control);

    control = CMenuPlayerInfo_FindControl(
        root, CSF_STR(0x008455EC, "BotonFeedBack"));
    self->field_0028 = reinterpret_cast<std::uint32_t>(control);
    if (control)
    {
        std::uint8_t enabled = 1;
        if (g_AccountMenuStateFlag && g_OnlineMatchFlag)
            enabled = 0;
        reinterpret_cast<void(__fastcall*)(void*, void*, std::uint8_t)>(
            CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, enabled);
    }

    control = CMenuPlayerInfo_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x008455D4, "EstaticoNombrePlayer"));
    self->field_002c = reinterpret_cast<std::uint32_t>(control);
    if (control)
    {
        char* source = reinterpret_cast<char*>(self->field_0048);
        if (CSF_AT(std::uint8_t, source, 0x200) & 0x40)
        {
            MenuMpAccounts_CopyWideString(
                reinterpret_cast<char*>(playerName), source + 2);
            int length = MenuMpAccounts_GetWideStringLength(playerName);
            playerName[length - 1] = 0;
        }
        else
        {
            MenuMpAccounts_CopyWideString(
                reinterpret_cast<char*>(playerName), source);
        }

        C_00720DF0_ForwardCallToChildVtblSlot16(
            reinterpret_cast<void*>(self->field_002c), nullptr, playerName);
    }

    control = CMenuPlayerInfo_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x008455C0, "EstaticoEstadoVoice"));
    self->field_0030 = reinterpret_cast<std::uint32_t>(control);
    if (control)
        CMenuPlayerInfo_SetFriendRequestTextByState(self, nullptr);

    root = reinterpret_cast<void*>(self->field_001c);
    reinterpret_cast<void(__fastcall*)(CMenuPlayerInfo*, void*, void*)>(
        CSF_AT(void**, self, 0)[0x44 / 4])(self, nullptr, root);
    CMenuPlayerInfo_RefreshScreen(self, nullptr);

    if (g_AccountMenuStateFlag && g_OnlineMatchFlag)
    {
        void* state = GetGlobalState();
        void* accountManager =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, state, 0)[0xF4 / 4])(state, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, accountManager, 0)[0x18 / 4])(
                accountManager, nullptr, 1);
    }

    control = reinterpret_cast<void*>(self->field_0020);
    if (!control)
        control = reinterpret_cast<void*>(self->field_0024);
    if (control)
    {
        root = reinterpret_cast<void*>(self->field_001c);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, root, 0)[0x158 / 4])(root, nullptr, control);
    }

    return static_cast<int>(self->field_001c);
}

HOOK(0x0055D940, CMenuPlayerInfo_InitializePlayerControls, "auto");
