#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_SelectedAccountIndex, 0x0089A5E8);

CSF_FUNC(void*, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x0050D190, ());
CSF_FUNC(std::uint32_t, __fastcall, MenuMpAccounts_DisplayAccountMessage,
         0x0050F790,
         (void* self, void* edx, int message, int option1, int option2, int option3));

static std::uint32_t __fastcall CMenuAccountEdit_SaveAccountChanges(
    CMenuAccountEdit* self, void* /*edx*/)
{
    const bool creatingAccount = g_SelectedAccountIndex == -1;
    void* accountSystem = GetGlobalValue_8B056C();
    void* accountService = CSF_AT(void*, accountSystem, 0x14);
    void* accountData = reinterpret_cast<std::uint8_t*>(self) + 0x34;

    unsigned char saved;
    if (creatingAccount)
    {
        saved = reinterpret_cast<unsigned char(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, accountService, 0)[0x18 / 4])(
                accountService, nullptr, accountData);
    }
    else
    {
        const int accountIndex = g_SelectedAccountIndex;
        saved = reinterpret_cast<unsigned char(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, accountService, 0)[0x1C / 4])(
                accountService, nullptr, accountIndex, accountData);
    }

    if (!saved)
    {
        if (creatingAccount)
            self->field_0280 = 3;

        accountSystem = GetGlobalValue_8B056C();
        accountService = CSF_AT(void*, accountSystem, 0x14);
        const int errorMessage =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, accountService, 0)[0x54 / 4])(
                    accountService, nullptr);

        void* globalState = GetGlobalState();
        const std::uint32_t result = MenuMpAccounts_DisplayAccountMessage(
            globalState, nullptr, errorMessage, 1, 1, 0);

        if (!creatingAccount)
            self->field_0280 = 3;

        return result;
    }

    const int successMessage = creatingAccount ? 0x41453131 : 0x41453137;
    void* globalState = GetGlobalState();
    const std::uint32_t result = MenuMpAccounts_DisplayAccountMessage(
        globalState, nullptr, successMessage, 1, 1, 0);
    self->field_0280 = 4;
    return result;
}

HOOK(0x0055C360, CMenuAccountEdit_SaveAccountChanges, "auto");
