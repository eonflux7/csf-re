#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, HudAlertIcons_IsSelectionTargetBlocked, 0x004A1960, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, HudAlertIcons_GetTargetAssignment, 0x004CFAB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(bool, __stdcall, MpSpawn_HasTeamReachedSpawnLimit, 0x00551730, (int teamSlot));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));

CSF_GLOBAL(float, g_CharacterFacingDistance, 0x00833854);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall HudAlertIcons_EvaluateSelectionStageTwo(void* self, void* /*edx*/)
{
    void* selection = CSF_AT(void*, self, 0x8);
    if (CSF_AT(int, selection, 0x298) != 0 ||
        CSF_AT(int, selection, 0x248) < 0)
        return;

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (CSF_AT(uint8_t, globalState, 0xD0) == 0)
        return;

    selection = CSF_AT(void*, self, 0x8);
    void* target = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(static_cast<uint8_t*>(selection) + 0x240)));
    if (!target || CSF_AT(int, target, 0x50) != 32)
        return;

    selection = CSF_AT(void*, self, 0x8);
    if (!(CSF_AT(float, selection, 0x258) <= g_CharacterFacingDistance))
        return;

    void* targetAssignment = CSF_AT(void*, target, 0x34);
    uint8_t* targetComponent = CSF_AT(uint8_t*, target, 0x18);
    void* targetCharacter = targetComponent ? targetComponent - 128 : nullptr;

    uint8_t* ownerComponent = CSF_AT(uint8_t*, selection, 0x18);
    void* ownerCharacter = ownerComponent ? ownerComponent - 1712 : nullptr;

    if (!targetAssignment || !ownerCharacter || !targetCharacter)
        return;

    if (static_cast<uint8_t>(
            HudAlertIcons_IsSelectionTargetBlocked(targetCharacter, nullptr)) != 0)
        return;

    const uint8_t targetTeam = CSF_AT(uint8_t, targetCharacter, 0x50);
    const int8_t ownerTeam = CSF_AT(int8_t, ownerCharacter, 0x443);
    if (targetTeam == static_cast<uint8_t>(ownerTeam))
        return;

    GetGlobal8B0688();
    if (!MpSpawn_HasTeamReachedSpawnLimit(static_cast<int>(ownerTeam)))
    {
        const int description = MenuExtras_ParseDescriptionId(CSF_STR(0x008344A8, "G073"));
        CSF_AT(int, CSF_AT(void*, self, 0x8), 0x2AC) = description;
        return;
    }

    alignas(8) uint8_t assignmentBytes[0x10];
    uint32_t* assignment = HudAlertIcons_GetTargetAssignment(
        targetAssignment, nullptr, reinterpret_cast<uint32_t*>(assignmentBytes));

    int description;
    if (assignment[1] == static_cast<uint32_t>(-1))
    {
        if (CSF_AT(uint8_t, targetCharacter, 0x44) != 0)
        {
            description = MenuExtras_ParseDescriptionId(CSF_STR(0x00834490, "G071"));
        }
        else
        {
            CSF_AT(int, CSF_AT(void*, self, 0x8), 0x298) = 14;
            selection = CSF_AT(void*, self, 0x8);
            CSF_AT(int, selection, 0x2A0) = CSF_AT(int, selection, 0x244);
            CSF_AT(int, selection, 0x2A4) = CSF_AT(int, selection, 0x248);
            description = MenuExtras_ParseDescriptionId(CSF_STR(0x00834488, "G063"));
        }
    }
    else
    {
        if (CSF_AT(uint8_t, targetCharacter, 0x44) == 0)
        {
            description = MenuExtras_ParseDescriptionId(CSF_STR(0x00834498, "G072"));
            CSF_AT(int, CSF_AT(void*, self, 0x8), 0x2AC) = description;
            return;
        }
        description = MenuExtras_ParseDescriptionId(CSF_STR(0x008344A0, "G082"));
    }

    CSF_AT(int, CSF_AT(void*, self, 0x8), 0x2AC) = description;
}

HOOK(0x004A1D70, HudAlertIcons_EvaluateSelectionStageTwo, "auto");
