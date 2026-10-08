#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_GLOBAL(uint8_t, g_AccountMenuStateFlag, 0x008B0901);
CSF_GLOBAL(uint8_t, g_AccountMenuSecondaryFlag, 0x008B068C);
CSF_GLOBAL(const char*, g_pszMenu_P_LbFLIn, 0x00899B64);

static void* (__cdecl* const GetGlobalState)() =
    reinterpret_cast<void* (__cdecl*)()>(CSF_ADDR_FN(0x006A6D70));

static int __fastcall MenuMpAccounts_InitializePlayerList(
    void* self, void* /*edx*/)
{
    if (g_AccountMenuStateFlag && g_AccountMenuSecondaryFlag)
    {
        void* state = GetGlobalState();
        void* controller =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, state, 0)[0xF4 / 4])(state, nullptr);

        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, controller, 0)[0x18 / 4])(
                controller, nullptr, 0);
    }

    void* menu = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x14));
    reinterpret_cast<void (__fastcall*)(void*, void*, void*, int)>(
        CSF_AT(void**, menu, 0)[0x60 / 4])(menu, nullptr, self, 0);

    const uint32_t result = CSF_AT(uint32_t, self, 0x18);
    if (result)
        return static_cast<int32_t>(result);

    menu = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x14));
    const char* label = g_pszMenu_P_LbFLIn;
    return reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, menu, 0)[0x58 / 4])(menu, nullptr, label);
}

HOOK(0x0055D470, MenuMpAccounts_InitializePlayerList, "auto");
