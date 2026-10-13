#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuMpAccounts_FindAccountByName, 0x0055BCB0, (void* self, void* , const char* name));
CSF_FUNC(int, __cdecl, GetWideTextLength, 0x00695560, (std::uint16_t* text));
CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620, (std::uint16_t* source, char* destination, unsigned int length));
CSF_FUNC(int16_t, __cdecl, CoreVariantValues_CopyWideString, 0x00695680, (char* dest, char* src));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(void*, __fastcall, C_00861120_GetSchedulerProxy, 0x00734A10, (C_00861120* self, void*));

static void* __fastcall MenuMpAccounts_CommitAccountSelection(void* self, void* /*edx*/)
{
    void* result = C_00861120_GetSchedulerProxy(
        reinterpret_cast<C_00861120*>(GetGlobalState_50D190()), nullptr);

    if (result == reinterpret_cast<void*>(4))
    {
        alignas(8) std::uint8_t wideBytes[0x200];
        alignas(8) std::uint8_t nameBytes[0x100];
        auto* wideText = reinterpret_cast<std::uint16_t*>(wideBytes);
        auto* accountName = reinterpret_cast<char*>(nameBytes);

        std::memset(accountName, 0, 0x100);

        char* source = CSF_AT(char*, self, 0x48);
        if ((CSF_AT(std::uint8_t, source, 0x200) & 0x40) != 0)
        {
            CoreVariantValues_CopyWideString(
                reinterpret_cast<char*>(wideBytes), source + 2);
            wideText[GetWideTextLength(wideText) - 1] = 0;
        }
        else
        {
            CoreVariantValues_CopyWideString(
                reinterpret_cast<char*>(wideBytes), source);
        }

        ConvertWideTextToNarrow(wideText, accountName, 256);

        void* globalState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        void* accountManager =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, globalState, 0)[0xF4 / 4])(
                    globalState, nullptr);

        int account = MenuMpAccounts_FindAccountByName(
            accountManager, nullptr, accountName);

        globalState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        accountManager =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, globalState, 0)[0xF4 / 4])(
                    globalState, nullptr);

        result =
            ((void* (__fastcall*)(void*, void*, int))
                CSF_AT(void**, accountManager, 0)[0x34 / 4])(
                    accountManager, nullptr, account);

        CSF_AT(std::uint32_t, self, 0x38) = 0;
        CSF_AT(std::uint32_t, self, 0x40) = 0;
    }
    else if (result == reinterpret_cast<void*>(8))
    {
        CSF_AT(std::uint32_t, self, 0x38) = 0;
    }

    return result;
}

HOOK(0x0055D730, MenuMpAccounts_CommitAccountSelection, "auto");
