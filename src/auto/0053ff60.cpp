#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MenuMissionInfo_IsDefaultDestinationFlagSet, 0x0053D690, ());
CSF_FUNC(int, __fastcall, CMenuMissionInfo_EnterPanelState, 0x0053D740, (CMenuMissionInfo* self, void* , int));
CSF_FUNC(void, __fastcall, CMenuMissionInfo_ProcessPendingPageStep, 0x0053E9A0, (CMenuMissionInfo* self, void*));
CSF_FUNC(char, __fastcall, CMenuMissionInfo_TryAdvanceToNextPage, 0x0053E9E0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(int, __cdecl, Sub_7A8ED0, 0x007A8ED0, (int a2));

CSF_GLOBAL(const char*, g_pszMenu_P_LoGame, 0x00899B08);

static void __fastcall CMenuMissionInfo_HandlePanelEvent(
    CMenuMissionInfo* self, void* /*edx*/, char* command, int /*unused*/)
{
    const uint8_t recognized = static_cast<uint8_t>(
        Sub_7A8ED0(static_cast<int>(reinterpret_cast<intptr_t>(command))));

    // The incoming this points four bytes past the complete panel object.
    auto* panel = reinterpret_cast<CMenuMissionInfo*>(
        reinterpret_cast<uint8_t*>(self) - 4);

    if (recognized)
    {
        if (!CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0010), 0x4))
        {
            panel->vUpdateAndForward(1);
            return;
        }

        CMenuMissionInfo_EnterPanelState(panel, nullptr, 1);
        return;
    }

    switch (reinterpret_cast<intptr_t>(command))
    {
    case 5:
        if (!CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0010), 0x4))
            panel->vUpdateAndForward(0);
        return;

    case 13:
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0010), 0x4))
            CMenuMissionInfo_EnterPanelState(panel, nullptr, 1);
        return;

    case 2:
        CMenuMissionInfo_ProcessPendingPageStep(panel, nullptr);
        return;

    case 3:
        CMenuMissionInfo_TryAdvanceToNextPage(panel, nullptr);
        return;

    case 6:
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xCC)
            && static_cast<uint8_t>(MenuMissionInfo_IsDefaultDestinationFlagSet()))
        {
            void* navigator = CSF_AT(void*, self, 0xC);
            reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
                CSF_AT(void**, navigator, 0)[0x58 / 4])(
                    navigator, nullptr, g_pszMenu_P_LoGame);
        }
        return;

    default:
        return;
    }
}

HOOK(0x0053FF60, CMenuMissionInfo_HandlePanelEvent, "auto");
