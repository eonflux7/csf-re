#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_SelectedAccountIndex, 0x0089A5E8);
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);

CSF_FUNC(void*, __cdecl, GetAccountManager, 0x00506BD0, ());
CSF_FUNC(void*, __cdecl, GetNetworkManager, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, GetMultiplayerService, 0x006A6D70, ());

CSF_FUNC(int, __fastcall, InitializeAccountRequest, 0x00501870,
         (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, DestroyAccountRequest, 0x004FF3C0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, RefreshNetworkAccountState, 0x0050E1E0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CheckNetworkAccountReadiness, 0x00734A10,
         (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, SendAccountRequest, 0x0050F790,
         (void* self, void* edx, int requestId, int flags,
          int mode, void* request));
CSF_FUNC(unsigned int, __fastcall, ShowAccountMessage, 0x0050F860,
         (void* self, void* edx, const void* message, int flags, int mode));
CSF_FUNC(char, __fastcall, CheckAccountName, 0x005184B0,
         (void* self, void* edx, const char* name, char mode));
CSF_FUNC(int, __fastcall, CommitAccountEdit, 0x0055C9D0,
         (void* self, void* edx));

CSF_FUNC(unsigned int, __cdecl, ConvertAccountNameToWide, 0x00695590,
         (const char* source, std::uint16_t* destination, int limit));
CSF_FUNC(int, __cdecl, FormatAccountMessage, 0x00692660,
         (void* destination, const char* key, const void* argument, int mode));
CSF_FUNC(int, __cdecl, FreeAccountMessageStorage, 0x00690E60,
         (void* block));

static char __fastcall MenuMpAccounts_SubmitAccountEdit(
    void* self, void* /*edx*/, int /*argument*/)
{
    auto* accountName = reinterpret_cast<char*>(self) + 0x3C;

    if (g_MpNetworkType == 3)
    {
        void* service = GetMultiplayerService();
        int status =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, service, 0)[0x1C / 4])(service, nullptr);

        if (status != 0)
        {
            if (status == 5)
            {
                void* manager = GetNetworkManager();
                status = CheckNetworkAccountReadiness(manager, nullptr);
                if (status != 0)
                {
                    service = GetMultiplayerService();
                    status =
                        reinterpret_cast<int(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, service, 0)[0x2C / 4])(
                                service, nullptr);
                    CSF_AT(int, self, 0x280) = 1;
                }
            }
            return static_cast<char>(status);
        }

        void* manager = GetNetworkManager();
        RefreshNetworkAccountState(manager, nullptr);

        service = GetMultiplayerService();
        const int requestId =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, service, 0)[0x20 / 4])(service, nullptr);

        if (requestId == 0x494C3538)
        {
            alignas(4) std::uint8_t request[180];
            InitializeAccountRequest(request, nullptr);
            request[0x18] = 1;

            manager = GetNetworkManager();
            SendAccountRequest(manager, nullptr, 0x41453230, 1, 0, request);

            service = GetMultiplayerService();
            reinterpret_cast<void(__fastcall*)(void*, void*, char*, char*)>(
                CSF_AT(void**, service, 0)[0x3C / 4])(
                    service, nullptr, accountName,
                    reinterpret_cast<char*>(self) + 0x5D);

            CSF_AT(int, self, 0x280) = 8;
            return static_cast<char>(DestroyAccountRequest(request, nullptr));
        }

        if (requestId != -1)
        {
            manager = GetNetworkManager();
            const unsigned int result =
                SendAccountRequest(manager, nullptr, requestId, 32, 1, nullptr);
            CSF_AT(int, self, 0x280) = 3;
            return static_cast<char>(result);
        }

        return static_cast<char>(CommitAccountEdit(self, nullptr));
    }

    alignas(4) std::uint8_t message[32];
    std::uint16_t wideName[34];
    char shouldCommit = 0;

    CSF_AT(std::uint32_t, message, 0x18) = 7;
    CSF_AT(std::uint32_t, message, 0x14) = 0;
    CSF_AT(std::uint16_t, message, 0x04) = 0;

    ConvertAccountNameToWide(accountName, wideName, 33);

    if (g_SelectedAccountIndex == -1 &&
        CheckAccountName(CSF_AT(void*, GetAccountManager(), 0x14),
                         nullptr, accountName, 1))
    {
        FormatAccountMessage(message, CSF_STR(0x008454F8, "ae24"),
                             wideName, 1);

        const std::uint32_t capacity =
            CSF_AT(std::uint32_t, message, 0x18);
        const void* messageText = CSF_AT(void*, message, 0x04);
        if (capacity < 8)
            messageText = message + 0x04;

        void* manager = GetNetworkManager();
        ShowAccountMessage(manager, nullptr, messageText, 1, 0);
        CSF_AT(int, self, 0x280) = 3;
    }
    else
    {
        shouldCommit = 1;
    }

    if (CSF_AT(std::uint32_t, message, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, message, 0x04);
        if (block)
            FreeAccountMessageStorage(block);
    }

    if (shouldCommit)
        return static_cast<char>(CommitAccountEdit(self, nullptr));

    return shouldCommit;
}

HOOK(0x0055CCC0, MenuMpAccounts_SubmitAccountEdit, "auto");
