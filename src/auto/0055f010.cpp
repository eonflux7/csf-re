#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(const char*, g_MessageBoxName, 0x00899B28);
CSF_GLOBAL(const char*, g_QuickMatchLabelName, 0x00899B4C);
CSF_GLOBAL(const char*, g_CreateMatchLabelName, 0x00899B50);
CSF_GLOBAL(const char*, g_OpenMatchLabelName, 0x00899B54);

static int __fastcall MenuMpOnline_ProcessPendingNavigation(
    void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x4C);
    if (!result)
    {
        void* menu = CSF_AT(void*, self, 0x3C);
        result = reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x5C / 4])(
                menu, nullptr, g_MessageBoxName);

        CSF_AT(int, self, 0x4C) = result;
        void* messageBox = reinterpret_cast<void*>(result);
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, messageBox, 0)[0x20 / 4])(
                messageBox, nullptr);
    }

    if (CSF_AT(std::uint8_t, self, 0x34))
    {
        void* menu = CSF_AT(void*, self, 0x3C);
        result = reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x58 / 4])(
                menu, nullptr, g_QuickMatchLabelName);
        CSF_AT(std::uint8_t, self, 0x34) = 0;
    }
    else if (CSF_AT(std::uint8_t, self, 0x35))
    {
        void* menu = CSF_AT(void*, self, 0x3C);
        result = reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x58 / 4])(
                menu, nullptr, g_OpenMatchLabelName);
        CSF_AT(std::uint8_t, self, 0x35) = 0;
    }
    else if (CSF_AT(std::uint8_t, self, 0x36))
    {
        void* menu = CSF_AT(void*, self, 0x3C);
        result = reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x58 / 4])(
                menu, nullptr, g_CreateMatchLabelName);
        CSF_AT(std::uint8_t, self, 0x36) = 0;
    }
    else
    {
        if (CSF_AT(std::uint8_t, self, 0x38))
            CSF_AT(std::uint8_t, self, 0x38) = 0;
        return result;
    }

    void* owner = CSF_AT(void*, self, 0x14);
    if (owner)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x16C / 4])(owner, nullptr);

        owner = CSF_AT(void*, self, 0x14);
        return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, owner, 0)[0xA8 / 4])(owner, nullptr, 1);
    }
    return result;
}

HOOK(0x0055F010, MenuMpOnline_ProcessPendingNavigation, "auto");
