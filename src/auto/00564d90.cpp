#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_SelectedAccountIndex, 0x0089A5E8);

CSF_FUNC(void*, __cdecl, MenuAccounts_GetAccountManager, 0x00506BD0, ());
CSF_FUNC(void*, __cdecl, MenuAccounts_GetGlobalState, 0x0050D190, ());
CSF_FUNC(unsigned int, __fastcall, MenuAccounts_ApplyAccountResult, 0x0050F790,
         (void* self, void* edx, int result, int flag1, int flag2, int flag3));

static __declspec(naked) int __fastcall MenuAccounts_TailDispatch(
    void* self, void* /*edx*/)
{
    __asm {
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0x60]
    }
}

static int __fastcall MenuAccounts_CommitAccountSelection(
    void* self, void* edx)
{
    CSF_AT(std::uint32_t, self, 0x28) = 1;

    void* manager = MenuAccounts_GetAccountManager();
    void* accounts = CSF_AT(void*, manager, 0x14);

    const std::uint8_t accepted =
        ((std::uint8_t(__fastcall*)(void*, void*, int))
             CSF_AT(void**, accounts, 0)[0x20 / 4])(
            accounts, nullptr, g_SelectedAccountIndex);

    if (!accepted)
    {
        CSF_AT(std::uint32_t, self, 0x28) = 3;

        manager = MenuAccounts_GetAccountManager();
        accounts = CSF_AT(void*, manager, 0x14);

        const int result =
            ((int(__fastcall*)(void*, void*))
                 CSF_AT(void**, accounts, 0)[0x54 / 4])(
                accounts, nullptr);

        void* state = MenuAccounts_GetGlobalState();
        MenuAccounts_ApplyAccountResult(state, nullptr, result, 1, 1, 0);
    }

    g_SelectedAccountIndex = -1;

    [[clang::musttail]] return MenuAccounts_TailDispatch(self, edx);
}

HOOK(0x00564D90, MenuAccounts_CommitAccountSelection, "auto");
