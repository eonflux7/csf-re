#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);

static void* (__fastcall* const GetEventActionObject)(void*, void*) =
    reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_ADDR_FN(0x006683F0));

static int __fastcall MenuMpAccounts_HandleAccountEvent(
    void* self, void* /*edx*/, int eventCode, uint32_t eventData)
{
    if (eventCode == 1)
    {
        void* owner = static_cast<uint8_t*>(self) - 4;
        return reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x4C / 4])(owner, nullptr);
    }

    if (eventCode == 28)
    {
        void* control =
            reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x280));
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, control, 0)[0x104 / 4])(control, nullptr);

        const uint32_t selectedAccount = CSF_AT(uint32_t, self, 0x1C);
        if (selectedAccount &&
            selectedAccount == static_cast<uint32_t>(result) &&
            g_MpNetworkType == 2)
        {
            CSF_AT(uint8_t, self, 0x2D) = 1;
        }
        return result;
    }

    if (eventCode == 50)
    {
        void* event = reinterpret_cast<void*>(eventData);
        if (CSF_AT(uint8_t, event, 0x38))
        {
            void* action = GetEventActionObject(self, nullptr);
            return reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, action, 0)[0x1C / 4])(action, nullptr);
        }
        return static_cast<int32_t>(eventData);
    }

    return static_cast<int32_t>(static_cast<uint32_t>(eventCode) - 50u);
}

HOOK(0x0055C100, MenuMpAccounts_HandleAccountEvent, "auto");
