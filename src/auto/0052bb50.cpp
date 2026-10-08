#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(void, __cdecl, Nullsub_506AC0,
         0x00506AC0, (int argument));
CSF_FUNC(std::uint32_t*, __fastcall, MenuMessageBox_AssignConfiguration,
         0x0050F160, (void* self, void* edx, int configuration));
CSF_FUNC(int, __fastcall, MenuMessageBox_ResetTextPlaceholders,
         0x00528A80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMessageBox_InitializeControls,
         0x005290B0, (void* self, void* edx));
CSF_FUNC(std::uint32_t*, __fastcall, CMenuMessageBox_RefreshLayoutAndCacheBounds,
         0x0052AE50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMessageBox_RefreshLayoutAndButtons,
         0x0052B650, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuMessageBox_PreCloseCleanup,
         0x0052B020, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMessageBox_ForwardToChildren,
         0x0052A7F0, (void* self, void* edx, int argument));

static void MenuMessageBox_SetChildState(void* child, std::uint8_t flag)
{
    // SETE AL retains the upper three bytes of the original child pointer.
    const std::uint32_t argument =
        (static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(child)) &
         0xFFFFFF00u) |
        static_cast<std::uint32_t>(flag == 0);

    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t)>(
        CSF_AT(void**, child, 0)[0xA8 / 4])(child, nullptr, argument);
}

static int __fastcall MenuMessageBox_ApplyConfiguration(
    void* self, void* /*edx*/, int configuration)
{
    MenuMessageBox_AssignConfiguration(
        static_cast<std::uint8_t*>(self) + 0x8A0, nullptr, configuration);
    MenuMessageBox_ResetTextPlaceholders(self, nullptr);

    if (CSF_AT(float, self, 0x8B4) == g_fZero &&
        CSF_AT(std::uint32_t, self, 0x7C) == 0)
    {
        Nullsub_506AC0(0);
    }

    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x48 / 4])(self, nullptr);

    void* panel = CSF_AT(void*, self, 0x894);
    if (panel)
    {
        const std::uint32_t argument = CSF_AT(std::uint32_t, self, 0x8B0);
        reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t)>(
            CSF_AT(void**, panel, 0)[0xEC / 4])(panel, nullptr, argument);
    }

    panel = CSF_AT(void*, self, 0x894);
    void* child =
        reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, panel, 0)[0x1C0 / 4])(panel, nullptr, 7);
    if (child)
    {
        void* method = CSF_AT(void**, child, 0)[0x88 / 4];
        const unsigned argument = CSF_AT(std::uint8_t, self, 0x8BA);
        reinterpret_cast<void (__fastcall*)(void*, void*, unsigned)>(
            method)(child, nullptr, argument);
    }

    child = CSF_AT(void*, self, 0x5C);
    if (child)
        MenuMessageBox_SetChildState(child, CSF_AT(std::uint8_t, self, 0x8B8));

    child = CSF_AT(void*, self, 0x60);
    if (child)
        MenuMessageBox_SetChildState(child, CSF_AT(std::uint8_t, self, 0x8B8));

    child = CSF_AT(void*, self, 0x64);
    if (child)
        MenuMessageBox_SetChildState(child, CSF_AT(std::uint8_t, self, 0x8B8));

    child = CSF_AT(void*, self, 0x68);
    if (child)
        MenuMessageBox_SetChildState(child, CSF_AT(std::uint8_t, self, 0x8B8));

    child = CSF_AT(void*, self, 0x6C);
    if (child)
        MenuMessageBox_SetChildState(child, CSF_AT(std::uint8_t, self, 0x8B9));

    child = CSF_AT(void*, self, 0x70);
    if (child)
        MenuMessageBox_SetChildState(child, CSF_AT(std::uint8_t, self, 0x8B9));

    child = CSF_AT(void*, self, 0x74);
    if (child)
        MenuMessageBox_SetChildState(child, CSF_AT(std::uint8_t, self, 0x8B9));

    CMenuMessageBox_InitializeControls(self, nullptr);
    CMenuMessageBox_RefreshLayoutAndCacheBounds(self, nullptr);
    CMenuMessageBox_RefreshLayoutAndButtons(self, nullptr);
    MenuMessageBox_PreCloseCleanup(self, nullptr);
    CMenuMessageBox_ForwardToChildren(self, nullptr, 0);

    const std::uint32_t state = CSF_AT(std::uint32_t, self, 0x8A8);
    panel = CSF_AT(void*, self, 0x894);
    void* method = CSF_AT(void**, panel, 0)[0x25C / 4];
    const std::uint32_t argument =
        (state & 0xFFFFFF00u) | static_cast<std::uint32_t>(state == 0);
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t)>(
        method)(panel, nullptr, argument);

    panel = CSF_AT(void*, self, 0x894);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, panel, 0)[0x16C / 4])(panel, nullptr);

    panel = CSF_AT(void*, self, 0x894);
    method = CSF_AT(void**, panel, 0)[0xC0 / 4];
    const unsigned finalArgument = CSF_AT(std::uint8_t, self, 0x948);
    return reinterpret_cast<int (__fastcall*)(void*, void*, unsigned)>(
        method)(panel, nullptr, finalArgument);
}

HOOK(0x0052BB50, MenuMessageBox_ApplyConfiguration, "auto");
