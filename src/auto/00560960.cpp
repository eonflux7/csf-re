#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440,
         (int textId, int variant));

using ModeDispatchTable = volatile std::uint32_t[6];
CSF_GLOBAL(ModeDispatchTable, g_MenuMpSessions_ModeDispatch, 0x00560BB8);

static void MenuMpSessions_SetControlFlag(void* control, std::uint32_t flag)
{
    ((void(__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, flag);
}

static void MenuMpSessions_SetLocalizedControlText(void* control, int textId)
{
    void* textObject =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, control, 0)[0x110 / 4])(control, nullptr);

    // The original captures this vtable before resolving the localized text.
    void** textVtable = CSF_AT(void**, textObject, 0);
    int text = MenuMessageBox_ResolveEntryByKey(textId, 1);
    ((void(__fastcall*)(void*, void*, int))
        textVtable[0x40 / 4])(textObject, nullptr, text);
}

static void MenuMpSessions_UpdateThreeStateText(
    void* control, std::uint32_t state)
{
    switch (state)
    {
    case 0:
        MenuMpSessions_SetLocalizedControlText(control, 0x4F4D3138);
        break;
    case 1:
        MenuMpSessions_SetLocalizedControlText(control, 0x4F4D3136);
        break;
    case 2:
        MenuMpSessions_SetLocalizedControlText(control, 0x4F4D3137);
        break;
    }
}

static void __fastcall CMenuMpSessions_RefreshOptionControls(
    CMenuMpSessions* self, void* /*edx*/)
{
    void* control = reinterpret_cast<void*>(self->field_0070);
    if (control)
        MenuMpSessions_SetControlFlag(control, 0);

    control = reinterpret_cast<void*>(self->field_0034);
    if (control)
        MenuMpSessions_SetControlFlag(control, 0);

    control = reinterpret_cast<void*>(self->field_004c);
    if (control)
    {
        std::uint32_t mode = self->field_0018;
        if (mode <= 5)
        {
            // Preserve the original dispatch-table memory reference.
            (void)g_MenuMpSessions_ModeDispatch[mode];
            switch (mode)
            {
            case 0:
                MenuMpSessions_SetLocalizedControlText(control, 0x434D3031);
                break;
            case 1:
                MenuMpSessions_SetLocalizedControlText(control, 0x434D3032);
                break;
            case 3:
                MenuMpSessions_SetLocalizedControlText(control, 0x434D3034);
                break;
            case 5:
                MenuMpSessions_SetLocalizedControlText(control, 0x4F4D3234);
                break;
            }
        }

        bool isFirstMode = self->field_0018 == 0;
        control = reinterpret_cast<void*>(self->field_006c);

        // The original overwrites only the low byte of its saved ECX word.
        std::uint32_t flagWord =
            (static_cast<std::uint32_t>(
                 reinterpret_cast<std::uintptr_t>(self)) & 0xFFFFFF00u) |
            static_cast<std::uint32_t>(isFirstMode);

        if (control)
            MenuMpSessions_SetControlFlag(control, flagWord);

        control = reinterpret_cast<void*>(self->field_0058);
        if (control)
            MenuMpSessions_SetControlFlag(control, flagWord);

        if (isFirstMode)
            self->field_001c = 0;
    }

    control = reinterpret_cast<void*>(self->field_0050);
    if (control)
        MenuMpSessions_UpdateThreeStateText(control, self->field_01b8);

    control = reinterpret_cast<void*>(self->field_0054);
    if (control)
        MenuMpSessions_UpdateThreeStateText(control, self->field_01bc);

    control = reinterpret_cast<void*>(self->field_0058);
    if (control)
    {
        switch (self->field_001c)
        {
        case 0:
            MenuMpSessions_SetLocalizedControlText(control, 0x636D3332);
            break;
        case 1:
            MenuMpSessions_SetLocalizedControlText(control, 0x636D3330);
            break;
        case 2:
            MenuMpSessions_SetLocalizedControlText(control, 0x636D3331);
            break;
        }
    }

    control = reinterpret_cast<void*>(self->field_005c);
    if (control)
        MenuMpSessions_UpdateThreeStateText(control, self->field_01c0);

    control = reinterpret_cast<void*>(self->field_00a4);
    if (control)
        MenuMpSessions_SetControlFlag(control, 0);
}

HOOK(0x00560960, CMenuMpSessions_RefreshOptionControls, "auto");
