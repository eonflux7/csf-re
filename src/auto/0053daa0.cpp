#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(int, __fastcall, Campaign_FindMission, 0x0043E4B0, (void* self, void* , char* sceneName));
CSF_FUNC(uint32_t*, __fastcall, Campaign_ResolveCurrentPanelKey, 0x0043E5B0, (uint8_t* self, void*));
CSF_FUNC(int, __cdecl, MissionInfo_IsPrerequisiteMet, 0x0044FB00, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(int, __fastcall, S_00500350_GetFirstRecordNameBuffer, 0x00500350, (S_00500350* self, void*));
CSF_FUNC(void, __fastcall, GlobalMenu_SetSelectedScenarioName, 0x00501990, (S_004ff0f0* self, void* , char* String1, char a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
[[maybe_unused]] static const auto g_RecordNameVtable = CSF_VTBL(0x00500350);

static hook::Tail __fastcall CMenuMissionInfo_ResetPanels(
    void* self, const uint32_t* /*args*/)
{
    do
    {
        if (static_cast<uint8_t>(MissionInfo_IsPrerequisiteMet()) == 0)
            break;

        void* menu = reinterpret_cast<void*>(GetGlobalValue_8B0568());
        if (!UmsVirtualProcessor_GetDefaultDestination(menu, nullptr))
            break;

        menu = reinterpret_cast<void*>(GetGlobalValue_8B0568());
        void* destination = UmsVirtualProcessor_GetDefaultDestination(menu, nullptr);
        if (CSF_AT(uint32_t, destination, 0x1C) != 0)
            break;

        void* settings = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
        if (CSF_AT(uint8_t, settings, 0xF1) == 0)
            break;

        auto* records = reinterpret_cast<S_00500350*>(GetGlobalValue_8B0568());
        char* sceneName = reinterpret_cast<char*>(
            S_00500350_GetFirstRecordNameBuffer(records, nullptr));

        void* campaign = reinterpret_cast<void*>(GetGlobal8afc7c());
        void* mission = reinterpret_cast<void*>(
            Campaign_FindMission(campaign, nullptr, sceneName));

        if (mission && CSF_AT(uint8_t, mission, 0xC0) != 0)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, self, 0)[0x48 / 4])(self, nullptr);

            int stateValue = GetGlobalState_50D190();
            void* state = reinterpret_cast<void*>(stateValue);
            void* panelState = CSF_AT(void*, state, 0x18);
            CSF_AT(uint8_t, panelState, 0x187) = 1;
            return hook::tail_ret(stateValue);
        }

        auto* currentCampaign = reinterpret_cast<uint8_t*>(GetGlobal8afc7c());
        char* panelKey = reinterpret_cast<char*>(
            Campaign_ResolveCurrentPanelKey(currentCampaign, nullptr));

        // The original compares exactly one byte, not the whole string.
        if (!panelKey || CSF_AT(uint8_t, panelKey, 0) == g_byte_82E65C)
            break;

        auto* globalMenu = reinterpret_cast<S_004ff0f0*>(GetGlobalValue_8B0568());
        GlobalMenu_SetSelectedScenarioName(globalMenu, nullptr, panelKey, 1);
        return hook::tail_ret(0);
    } while (false);

    return hook::tail_jump(self, (*reinterpret_cast<void***>(self))[0x44 / 4]);
}

HOOK_TAIL(0x0053DAA0, CMenuMissionInfo_ResetPanels, "menu_mission_info");
