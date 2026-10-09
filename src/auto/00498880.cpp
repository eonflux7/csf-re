#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_SelectionRefreshInterval, 0x00833E34);

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(void, __fastcall, C_42def0_ClearByteFlag, 0x0042DEF0, (void* self, void*));
CSF_FUNC(char, __fastcall, Hud_RefreshObjectPanel, 0x0042F700, (void* self, void* , int handle));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));

CSF_FUNC(int, __fastcall, SceneObject_GetSelectionIndex, 0x0048E780, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00833ee8_SelectStoredTarget, 0x00493E70, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00833ee8_EvaluateInteractionTarget, 0x00493EB0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00833ee8_EvaluatePrimaryTarget, 0x004940A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00833ee8_EvaluateFallbackTarget, 0x004941D0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00833ee8_EvaluateTargetStageOne, 0x00494250, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00833ee8_EvaluateTargetStageTwo, 0x00494360, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00833ee8_EvaluateTargetStageThree, 0x004944E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00833ee8_FinalizeTargetSelection, 0x004945A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00833ee8_EvaluateFinalTarget, 0x00494700, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00833ee8_EvaluateTargetStageFour, 0x00496590, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00833ee8_EvaluateTargetStageFive, 0x00496750, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, C_00833ee8_EvaluateSpecialTarget, 0x004973E0, (void* self, void* edx));

CSF_FUNC(void, __fastcall, SelectionController_EvaluateStageOne, 0x004A1A20, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SelectionController_UpdateSelection, 0x004A1BB0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SelectionController_EvaluateStageTwo, 0x004A1D70, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SelectionController_EvaluateStageThree, 0x004A1F30, (void* self, void* edx));

CSF_FUNC(int, __cdecl, GetGlobal8B0558, 0x004F4920, ());
CSF_FUNC(void*, __fastcall, SelectionDisplay_SetSelectionData, 0x004F49E0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __stdcall, MenuMissionInfo_SetGlobalIndex, 0x00525DD0, (int a1));
CSF_FUNC(int, __cdecl, SelectionData_BuildFromIndex, 0x00698DF0, (int a1, int a2));

static void __fastcall C_00833ee8_RefreshTargetSelection(
    C_00833ee8* self, void* /*edx*/)
{
    if (reinterpret_cast<uint32_t>(self) !=
        GetSceneMenuContext(g_pCurrentScene, nullptr))
        return;

    // The original also proceeds when the comparison is unordered.
    if (self->field_0218 < g_SelectionRefreshInterval)
        return;

    const uint32_t previousIndex = self->field_02ac;
    self->field_0218 = 0.0f;
    self->field_0298 = 0;
    self->field_02a0 = -1;
    self->field_02a4 = -1;

    uint32_t selectionData[3] = {0, 0, previousIndex};

    self->field_02a8 = -1;
    self->field_02ac = -1;

    if ((self->field_02e0 & 0xFFFFF7FA) == 0)
    {
        C_00833ee8_EvaluatePrimaryTarget(self, nullptr);
        C_00833ee8_SelectStoredTarget(self, nullptr);
        // SelectStoredTarget leaves its receiver unchanged in ECX.
        C_00833ee8_EvaluateInteractionTarget(self, nullptr);
        C_00833ee8_EvaluateTargetStageOne(self, nullptr);
        C_00833ee8_EvaluateTargetStageTwo(self, nullptr);
        C_00833ee8_EvaluateTargetStageThree(self, nullptr);
        C_00833ee8_EvaluateTargetStageFour(self, nullptr);
        C_00833ee8_EvaluateTargetStageFive(self, nullptr);
        C_00833ee8_EvaluateSpecialTarget(self, nullptr);
        C_00833ee8_EvaluateFallbackTarget(self, nullptr);
        SelectionController_EvaluateStageOne(
            reinterpret_cast<void*>(self->field_03e8), nullptr);
        SelectionController_EvaluateStageTwo(
            reinterpret_cast<void*>(self->field_03e8), nullptr);
        SelectionController_EvaluateStageThree(
            reinterpret_cast<void*>(self->field_03e8), nullptr);
        C_00833ee8_EvaluateFinalTarget(self, nullptr);
        C_00833ee8_FinalizeTargetSelection(self, nullptr);
    }

    if ((self->field_02e0 & 0x10000) != 0)
        C_00833ee8_EvaluateSpecialTarget(self, nullptr);

    SelectionController_UpdateSelection(
        reinterpret_cast<void*>(self->field_03e8), nullptr);

    if (self->field_0298 == 8)
    {
        const int handle = reinterpret_cast<int>(&self->field_029c);
        void* hud = reinterpret_cast<void*>(GetGlobal8afc78());
        Hud_RefreshObjectPanel(hud, nullptr, handle);
    }
    else
    {
        void* hud = reinterpret_cast<void*>(GetGlobal8afc78());
        C_42def0_ClearByteFlag(hud, nullptr);
    }

    if (self->field_02ac == static_cast<uint32_t>(-1))
        return;

    if ((self->field_02a4 & 0x80000000) == 0)
    {
        const int target = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(&self->field_029c));
        if (target)
        {
            const int index = SceneObject_GetSelectionIndex(
                reinterpret_cast<void*>(target), nullptr);
            if (index != -1)
                self->field_02ac = index;
        }
    }

    if (self->field_02ac != previousIndex)
    {
        Get_008b0678();
        MenuMissionInfo_SetGlobalIndex(98);
    }

    SelectionData_BuildFromIndex(
        static_cast<int>(self->field_02ac),
        reinterpret_cast<int>(selectionData));
    void* display = reinterpret_cast<void*>(GetGlobal8B0558());
    SelectionDisplay_SetSelectionData(display, nullptr, selectionData);
}

HOOK(0x00498880, C_00833ee8_RefreshTargetSelection, "auto");
