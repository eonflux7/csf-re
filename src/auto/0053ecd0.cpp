#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(int, __fastcall, Campaign_FindMission, 0x0043E4B0, (void* self, void* , char* sceneName));
CSF_FUNC(double, __fastcall, MissionInfo_GetHitScore, 0x00444D30, (void* self, void* edx));
CSF_FUNC(double, __fastcall, MissionInfo_GetKillScore, 0x00444D60, (void* self, void* edx));
CSF_FUNC(double, __fastcall, MissionInfo_GetEliminationScore, 0x00444D90, (void* self, void* edx));
CSF_FUNC(double, __fastcall, MissionInfo_GetOfficerScore, 0x00444DC0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, MissionInfo_GetSpecialEnemyScore, 0x00444DF0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, MissionInfo_GetHeavyEnemyScore, 0x00444E20, (void* self, void* edx));
CSF_FUNC(double, __fastcall, MissionInfo_GetEliteEnemyScore, 0x00444E50, (void* self, void* edx));
CSF_FUNC(double, __fastcall, ComputeColorAnimValue, 0x00444E80, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, C_Unknown_GetMember0170, 0x0044F640, (char* self, void*));
CSF_FUNC(int, __cdecl, MissionInfo_IsPrerequisiteMet, 0x0044FB00, ());
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_GetByte023C, 0x00495490, (char* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, S_00500350_GetFirstRecordNameBuffer, 0x00500350, (S_00500350* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float, __fastcall, MenuSystem_GetPanelTransitionScale, 0x0050D950, (void* self, void*));
CSF_FUNC(void, __fastcall, MenuManager_SetMenuState, 0x0050DC80, (void* self, void* edx, char flag));
CSF_FUNC(int*, __fastcall, ProfileSavegames_GetScenarioCollection, 0x00515CE0, (void* self, void*));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, C_00840fdc_ClearSceneResources, 0x00520420, (C_00840fdc* self, void* , char clearOptionalResources));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __stdcall, MenuMissionInfo_SetGlobalIndex, 0x00525DD0, (int a1));
CSF_FUNC(int, __fastcall, CMenuOptions_ApplyTargetOffset, 0x0053AD90, (CMenuOptions* self, void*));
CSF_FUNC(void*, __fastcall, MenuMissionInfo_AssignFromArray, 0x0053D6D0, (void* self, void* , void* source));
CSF_FUNC(int, __fastcall, MenuMissionInfo_ComputeScaledLevel, 0x0053DB70, (void* self, void*));
CSF_FUNC(int, __fastcall, CMenuMissionInfo_PreparePanels, 0x0053DD30, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MissionInfo_GetBestMissionScore, 0x0053DE30, ());
CSF_FUNC(char, __fastcall, CMenuMissionInfo_CheckSavedScenario, 0x0053E0B0, (void* self, void* edx, int a2));
CSF_FUNC(char, __cdecl, MissionInfo_IsNextMissionUnavailable, 0x0053E230, ());
CSF_FUNC(void, __fastcall, CMenuMissionInfo_RefreshPanelAtIndex, 0x0053E2F0, (CMenuMissionInfo* self, void* edx, int index));
CSF_FUNC(int, __stdcall, MenuMissionInfo_PopulateEntryScores, 0x0053EA30, (void* menu, char category, int , int));
CSF_FUNC(int, __cdecl, RwFreeListPurgeAllFreeLists, 0x006265F0, ());
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(int, __cdecl, MissionInfo_FormatScore, 0x006969C0, (int a1, int a2, void* a3, ...));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, MissionInfo_PurgeRenderResources, 0x006B16A0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void* MissionInfo_Pointer(uint32_t value)
{
    return reinterpret_cast<void*>(value);
}

static void* MissionInfo_FindControl(void* object, const char* name)
{
    return reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, object, 0)[0xFC / 4])(object, nullptr, name);
}

static void MissionInfo_SetVisible(void* object, int visible)
{
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, object, 0)[0xA8 / 4])(object, nullptr, visible);
}

static void MissionInfo_SetText(void* table, int row, int column,
                                const char* text, int style)
{
    reinterpret_cast<void (__fastcall*)(void*, void*, int, int, const char*, int)>(
        CSF_AT(void**, table, 0)[0x290 / 4])(
            table, nullptr, row, column, text, style);
}

static void MissionInfo_SetFormattedText(void* table, int row, int column,
                                         const void* text, int style)
{
    reinterpret_cast<void (__fastcall*)(void*, void*, int, int, const void*, int)>(
        CSF_AT(void**, table, 0)[0x294 / 4])(
            table, nullptr, row, column, text, style);
}

static void MissionInfo_SetLocalizedCell(void* table, int row, int code, int style)
{
    auto setCell =
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int, int, int)>(
            CSF_AT(void**, table, 0)[0x294 / 4]);
    int text = MenuMessageBox_ResolveEntryByKey(code, 1);
    setCell(table, nullptr, row, 0, text, style);
}

static uint32_t MissionInfo_TruncateScore(double value)
{
    return static_cast<uint32_t>(static_cast<int64_t>(value));
}

static void MissionInfo_WriteCount(void* table, int row, int column,
                                   char* buffer, uint32_t count, float maximum)
{
    _sprintf(buffer, CSF_STR(0x00842F4C, "%d x %d"),
             count, MissionInfo_TruncateScore(maximum));
    MissionInfo_SetText(table, row, column, buffer, 0);
}

static void MissionInfo_WriteScore(void* table, int row, int column,
                                   void* buffer, uint32_t score,
                                   int scoreLabel, int formattingMode, int style)
{
    MissionInfo_FormatScore(
        reinterpret_cast<int>(buffer), 400,
        const_cast<char*>(CSF_STR(0x00842F40, "{d0} {w1}")),
        score, scoreLabel, formattingMode);
    MissionInfo_SetFormattedText(table, row, column, buffer, style);
}

static bool MissionInfo_HasNormalDestination()
{
    if (!static_cast<uint8_t>(MissionInfo_IsPrerequisiteMet()))
        return false;
    if (!UmsVirtualProcessor_GetDefaultDestination(
            MissionInfo_Pointer(GetGlobalValue_8B0568()), nullptr))
        return false;
    return CSF_AT(int, UmsVirtualProcessor_GetDefaultDestination(
                           MissionInfo_Pointer(GetGlobalValue_8B0568()), nullptr),
                  0x1C) == 0;
}

static uint32_t __fastcall CMenuMissionInfo_ShowMissionReport(
    CMenuMissionInfo* self, void* /*edx*/, int screen, int /*lookupContext*/)
{
    void* activeObject =
        CSF_AT(void*, MissionInfo_Pointer(GetGlobalState_50D190()), 0x74);
    if (activeObject)
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, activeObject, 0)[0xC / 4])(activeObject, nullptr);

    if (!static_cast<uint8_t>(MissionInfo_IsPrerequisiteMet()))
        return 0;

    alignas(8) uint8_t percentageBytes[0xC8];
    alignas(8) uint8_t starNameBytes[0x34];
    alignas(8) uint8_t formattedBytes[0x190];
    alignas(8) uint8_t countBytes[0xC8];
    alignas(8) uint8_t entryArrayBytes[0x30];
    char* percentage = reinterpret_cast<char*>(percentageBytes);
    char* starName = reinterpret_cast<char*>(starNameBytes);
    char* countText = reinterpret_cast<char*>(countBytes);

    // The unsigned-to-double adjustment in the old code is implicit in C++.
    (void)&g_UnsignedIntegerBias;
    (void)CSF_VTBL(0x00500350);

    void* statistics = MissionInfo_Pointer(
        CGuiControl_GetColorAnim(
            reinterpret_cast<CGuiControl*>(GetGlobalValue_8B0568()), nullptr) + 8);

    MissionInfo_SetVisible(MissionInfo_Pointer(self->field_001c), 0);
    self->vResetState();

    void* context = MissionInfo_Pointer(self->field_0014);
    self->field_0010 = screen;
    int contextId = context ? CSF_AT(int, context, 0) : 0;
    void* root = MissionInfo_Pointer(self->field_001c);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, root, 0)[0xEC / 4])(root, nullptr, contextId);

    MenuProgressInput_UpdateGlobalState(
        MissionInfo_Pointer(GetGlobalValue_521BB0()), nullptr);

    root = MissionInfo_Pointer(self->field_001c);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, root, 0)[0x16C / 4])(root, nullptr);

    if (!CSF_AT(uint8_t, MissionInfo_Pointer(GetGlobalState_50D190()), 0x89))
        MenuManager_SetMenuState(
            MissionInfo_Pointer(GetGlobalState_50D190()), nullptr, 1);

    CMenuMissionInfo_PreparePanels(self, nullptr);

    void* tacticalMap = MissionInfo_FindControl(
        MissionInfo_Pointer(self->field_001c),
        CSF_STR(0x00842FE4, "PanelMapaTactico"));

    if (tacticalMap)
    {
        void* photo = MissionInfo_FindControl(
            tacticalMap, CSF_STR(0x00842FD4, "FotoCommandoBN"));
        if (photo)
            MissionInfo_SetVisible(photo, 0);
        photo = MissionInfo_FindControl(
            tacticalMap, CSF_STR(0x00842FC8, "FotoSpyBN"));
        if (photo)
            MissionInfo_SetVisible(photo, 0);
        photo = MissionInfo_FindControl(
            tacticalMap, CSF_STR(0x00842FB8, "FotoSniperBN"));
        if (photo)
            MissionInfo_SetVisible(photo, 0);

        void* sceneCharacters = static_cast<uint8_t*>(g_pCurrentScene) + 0x60;
        uint32_t index = 0;
        if (CSF_AT(uint32_t, sceneCharacters, 0x8))
        {
            do
            {
                void* character = CSF_AT(void**, sceneCharacters, 0)[index];
                const char* photoName = nullptr;
                switch (CSF_AT(int, character, 0x54))
                {
                case 1:
                    photoName = CSF_STR(0x00842FD4, "FotoCommandoBN");
                    break;
                case 2:
                    photoName = CSF_STR(0x00842FC8, "FotoSpyBN");
                    break;
                case 3:
                    photoName = CSF_STR(0x00842FB8, "FotoSniperBN");
                    break;
                }

                if (photoName)
                {
                    photo = MissionInfo_FindControl(tacticalMap, photoName);
                    if (photo)
                    {
                        auto showPhoto =
                            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                                CSF_AT(void**, photo, 0)[0xA8 / 4]);
                        char visible = C_GetByte023C(
                            static_cast<char*>(character), nullptr);
                        showPhoto(photo, nullptr, visible);

                        if (reinterpret_cast<uint32_t>(character) ==
                            GetSceneMenuContext(g_pCurrentScene, nullptr))
                        {
                            void* frame = MissionInfo_FindControl(
                                tacticalMap, CSF_STR(0x00842FAC, "MarcoFotos"));
                            if (frame)
                            {
                                auto setFrame =
                                    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                                        CSF_AT(void**, frame, 0)[0x98 / 4]);
                                int placement =
                                    reinterpret_cast<int (__fastcall*)(void*, void*)>(
                                        CSF_AT(void**, photo, 0)[0x9C / 4])(
                                            photo, nullptr);
                                setFrame(frame, nullptr, placement);
                                void* frameAppearance =
                                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                                        CSF_AT(void**, frame, 0)[0xDC / 4])(
                                            frame, nullptr);
                                if (frameAppearance)
                                {
                                    CSF_AT(float, entryArrayBytes, 0x0) = 136.0f;
                                    CSF_AT(float, entryArrayBytes, 0x4) = 80.0f;
                                    CSF_AT(float, entryArrayBytes, 0x8) = 18.0f;
                                    CSF_AT(float, entryArrayBytes, 0xC) = 255.0f;
                                    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                                        CSF_AT(void**, frameAppearance, 0)[0x68 / 4])(
                                            frameAppearance, nullptr, entryArrayBytes);
                                }
                            }
                        }
                    }
                }
                ++index;
            } while (index < CSF_AT(uint32_t, sceneCharacters, 0x8));
        }
    }

    int objectiveScore = 0;
    void* objectives = MissionInfo_FindControl(
        MissionInfo_Pointer(self->field_001c),
        CSF_STR(0x00842F9C, "PanelObjetivos"));
    if (objectives)
    {
        void* primary = MissionInfo_FindControl(
            objectives, CSF_STR(0x00842F8C, "ObjsPrincipales"));
        void* secondary = MissionInfo_FindControl(
            objectives, CSF_STR(0x00842F7C, "ObjsSecundarios"));
        CSF_AT(int, entryArrayBytes, 0x0) = 0;
        CSF_AT(int, entryArrayBytes, 0x4) = 0;
        CSF_AT(int, entryArrayBytes, 0x8) = 0;
        CSF_AT(int, entryArrayBytes, 0xC) = 10;
        objectiveScore = MenuMissionInfo_PopulateEntryScores(
            primary, 0, 0, reinterpret_cast<int>(entryArrayBytes));
        objectiveScore += MenuMissionInfo_PopulateEntryScores(
            secondary, 1, 0, reinterpret_cast<int>(entryArrayBytes));
        MenuMissionInfo_AssignFromArray(
            reinterpret_cast<uint8_t*>(self) + 0x94, nullptr, entryArrayBytes);
    }

    void* creatures = MissionInfo_FindControl(
        MissionInfo_Pointer(self->field_001c),
        CSF_STR(0x00842CD4, "PanelBichos"));
    int scoreLabel = MenuMessageBox_ResolveEntryByKey(1347310675, 1);
    void* killsReport = MissionInfo_FindControl(
        MissionInfo_Pointer(self->field_001c),
        CSF_STR(0x00842F68, "PanelKillsReport"));

    if (killsReport)
    {
        void* eliminated = MissionInfo_FindControl(
            killsReport, CSF_STR(0x00842F54, "EnemigosEliminados"));
        float maximum = CSF_AT(
            float, MissionInfo_Pointer(GetGlobalValue_8BC1E0()), 7500);
        MissionInfo_WriteCount(
            eliminated, 0, 0, countText,
            CSF_AT(uint32_t, statistics, 0x10), maximum);
        int mode = g_NumberFormattingMode;
        double score = MissionInfo_GetEliminationScore(statistics, nullptr);
        MissionInfo_WriteScore(
            eliminated, 0, 1, formattedBytes,
            MissionInfo_TruncateScore(score), scoreLabel, mode, 0);
    }

    void* hits = MissionInfo_FindControl(
        killsReport, CSF_STR(0x00842F30, "FichaAciertos"));
    if (hits)
    {
        MissionInfo_SetLocalizedCell(hits, 0, 1212437332, 0);
        MissionInfo_SetLocalizedCell(hits, 1, 1397311305, 0);

        float maximum = CSF_AT(
            float, MissionInfo_Pointer(GetGlobalValue_8BC1E0()), 7492);
        MissionInfo_WriteCount(
            hits, 0, 1, countText, CSF_AT(uint32_t, statistics, 0x38), maximum);
        int mode = g_NumberFormattingMode;
        double score = MissionInfo_GetKillScore(statistics, nullptr);
        MissionInfo_WriteScore(
            hits, 0, 2, formattedBytes,
            MissionInfo_TruncateScore(score), scoreLabel, mode, 0);

        maximum = CSF_AT(
            float, MissionInfo_Pointer(GetGlobalValue_8BC1E0()), 7496);
        MissionInfo_WriteCount(
            hits, 1, 1, countText, CSF_AT(uint32_t, statistics, 0x34), maximum);
        mode = g_NumberFormattingMode;
        score = MissionInfo_GetHitScore(statistics, nullptr);
        MissionInfo_WriteScore(
            hits, 1, 2, formattedBytes,
            MissionInfo_TruncateScore(score), scoreLabel, mode, 0);
    }

    void* specialEnemies = MissionInfo_FindControl(
        killsReport, CSF_STR(0x00842F1C, "EnemigosEspeciales"));
    if (specialEnemies)
    {
        MissionInfo_SetLocalizedCell(specialEnemies, 0, 1398096463, 0);
        MissionInfo_SetLocalizedCell(specialEnemies, 1, 1330006339, 0);
        MissionInfo_SetLocalizedCell(specialEnemies, 2, 1195724357, 0);
        MissionInfo_SetLocalizedCell(specialEnemies, 3, 1195725652, 0);

        float maximum = CSF_AT(
            float, MissionInfo_Pointer(GetGlobalValue_8BC1E0()), 7504);
        MissionInfo_WriteCount(
            specialEnemies, 0, 1, countText,
            CSF_AT(uint32_t, statistics, 0x18), maximum);
        int mode = g_NumberFormattingMode;
        double score = MissionInfo_GetOfficerScore(statistics, nullptr);
        MissionInfo_WriteScore(
            specialEnemies, 0, 2, formattedBytes,
            MissionInfo_TruncateScore(score), scoreLabel, mode, 0);

        maximum = CSF_AT(
            float, MissionInfo_Pointer(GetGlobalValue_8BC1E0()), 7508);
        MissionInfo_WriteCount(
            specialEnemies, 1, 1, countText,
            CSF_AT(uint32_t, statistics, 0x20), maximum);
        mode = g_NumberFormattingMode;
        score = MissionInfo_GetSpecialEnemyScore(statistics, nullptr);
        MissionInfo_WriteScore(
            specialEnemies, 1, 2, formattedBytes,
            MissionInfo_TruncateScore(score), scoreLabel, mode, 0);

        maximum = CSF_AT(
            float, MissionInfo_Pointer(GetGlobalValue_8BC1E0()), 7512);
        MissionInfo_WriteCount(
            specialEnemies, 2, 1, countText,
            CSF_AT(uint32_t, statistics, 0x28), maximum);
        mode = g_NumberFormattingMode;
        score = MissionInfo_GetHeavyEnemyScore(statistics, nullptr);
        MissionInfo_WriteScore(
            specialEnemies, 2, 2, formattedBytes,
            MissionInfo_TruncateScore(score), scoreLabel, mode, 0);

        maximum = CSF_AT(
            float, MissionInfo_Pointer(GetGlobalValue_8BC1E0()), 7516);
        MissionInfo_WriteCount(
            specialEnemies, 3, 1, countText,
            CSF_AT(uint32_t, statistics, 0x30), maximum);
        mode = g_NumberFormattingMode;
        score = MissionInfo_GetEliteEnemyScore(statistics, nullptr);
        MissionInfo_WriteScore(
            specialEnemies, 3, 2, formattedBytes,
            MissionInfo_TruncateScore(score), scoreLabel, mode, 0);
    }

    void* accuracy = MissionInfo_FindControl(
        killsReport, CSF_STR(0x00842F08, "EfectividadEnTiro"));
    if (accuracy)
    {
        MissionInfo_SetLocalizedCell(accuracy, 0, 1162233155, -1);
        MissionInfo_SetLocalizedCell(accuracy, 1, 1094926658, -1);
        MissionInfo_SetLocalizedCell(accuracy, 2, 1094996820, -1);

        uint32_t shots = CSF_AT(uint32_t, statistics, 0x3C);
        double percent = shots
            ? static_cast<double>(CSF_AT(uint32_t, statistics, 0x40)) /
                  static_cast<double>(shots) * 100.0
            : g_fZero;
        _sprintf(percentage, CSF_STR(0x00842F00, "%.1f%%"), percent);
        MissionInfo_SetText(accuracy, 0, 1, percentage, -1);

        shots = CSF_AT(uint32_t, statistics, 0x3C);
        percent = shots
            ? static_cast<double>(CSF_AT(uint32_t, statistics, 0x38)) /
                  static_cast<double>(shots) * 100.0
            : g_fZero;
        if (shots && percent > g_MaxPercent)
            percent = g_MaxPercent;
        _sprintf(percentage, CSF_STR(0x00842F00, "%.1f%%"), percent);
        MissionInfo_SetText(accuracy, 1, 1, percentage, -1);

        shots = CSF_AT(uint32_t, statistics, 0x3C);
        percent = shots
            ? static_cast<double>(CSF_AT(uint32_t, statistics, 0x44)) /
                  static_cast<double>(shots) * 100.0
            : g_fZero;
        _sprintf(percentage, CSF_STR(0x00842F00, "%.1f%%"), percent);
        MissionInfo_SetText(accuracy, 2, 1, percentage, -1);
    }

    void* overall = MissionInfo_FindControl(
        MissionInfo_Pointer(self->field_001c),
        CSF_STR(0x00842EF0, "PanelOverall"));
    if (overall)
    {
        uint32_t unscaledScore =
            objectiveScore + MissionInfo_TruncateScore(
                ComputeColorAnimValue(statistics, nullptr));
        void* globalState = MissionInfo_Pointer(GetGlobalState_50D190());
        uint32_t totalScore = MissionInfo_TruncateScore(
            MenuSystem_GetPanelTransitionScale(globalState, nullptr) *
            static_cast<double>(unscaledScore));

        void* totals = MissionInfo_FindControl(
            overall, CSF_STR(0x00842EE0, "PuntTotalMision"));
        MissionInfo_SetLocalizedCell(totals, 0, 1346457410, -1);
        MissionInfo_SetLocalizedCell(totals, 1, 1346456393, -1);
        MissionInfo_SetLocalizedCell(totals, 2, 1346652486, -1);
        MissionInfo_SetLocalizedCell(totals, 3, 1347764547, -1);
        MissionInfo_SetLocalizedCell(totals, 4, 1297629518, -1);

        MissionInfo_WriteScore(
            totals, 0, 1, formattedBytes, objectiveScore,
            scoreLabel, g_NumberFormattingMode, -1);
        int mode = g_NumberFormattingMode;
        double combatScore = ComputeColorAnimValue(statistics, nullptr);
        MissionInfo_WriteScore(
            totals, 1, 1, formattedBytes,
            MissionInfo_TruncateScore(combatScore), scoreLabel, mode, -1);

        globalState = MissionInfo_Pointer(GetGlobalState_50D190());
        double multiplier =
            MenuSystem_GetPanelTransitionScale(globalState, nullptr);
        _sprintf(percentage, CSF_STR(0x00842ED8, "%.2f"), multiplier);
        MissionInfo_SetText(totals, 2, 1, percentage, -1);
        MissionInfo_WriteScore(
            totals, 3, 1, formattedBytes, totalScore,
            scoreLabel, g_NumberFormattingMode, -1);

        self->field_0090 = MissionInfo_GetBestMissionScore();
        self->field_008c = totalScore;
        uint32_t bestScore = self->field_008c;
        if (self->field_0090 >= totalScore)
            bestScore = self->field_0090;
        MissionInfo_WriteScore(
            totals, 4, 1, formattedBytes, bestScore,
            scoreLabel, g_NumberFormattingMode, -1);

        uint32_t stars = 0;
        if (CSF_AT(uint8_t, MissionInfo_Pointer(self->field_0014), 0x4) ||
            MissionInfo_HasNormalDestination())
            stars = MenuMissionInfo_ComputeScaledLevel(self, nullptr);

        for (uint32_t index = 0; index < 5; ++index)
        {
            _sprintf(starName, CSF_STR(0x008415F0, "Estrella%d"), index + 1);
            void* star = MissionInfo_FindControl(overall, starName);
            if (star)
                MissionInfo_SetVisible(star, index < stars);
        }

        void* tips = MissionInfo_FindControl(
            overall, CSF_STR(0x00842ED0, "Tips"));
        if (tips)
        {
            void* tipKeys = C_Unknown_GetMember0170(
                static_cast<char*>(g_pCurrentScene), nullptr);
            uint32_t index = 0;
            if (CSF_AT(uint32_t, tipKeys, 0x8))
            {
                do
                {
                    int text = MenuMessageBox_ResolveEntryByKey(
                        CSF_AT(int*, tipKeys, 0)[index], 12);
                    MissionInfo_SetFormattedText(
                        tips, index, 0, MissionInfo_Pointer(text), -1);
                    ++index;
                } while (index < CSF_AT(uint32_t, tipKeys, 0x8));
            }
        }
    }

    self->field_0088 = 0;
    CMenuMissionInfo_RefreshPanelAtIndex(self, nullptr, 0);
    if (self->field_0084)
        MissionInfo_SetVisible(MissionInfo_Pointer(self->field_0084), 1);
    if (tacticalMap)
        MissionInfo_SetVisible(tacticalMap, 1);
    if (objectives)
        MissionInfo_SetVisible(objectives, 1);
    if (creatures)
        MissionInfo_SetVisible(creatures, 1);
    MissionInfo_SetVisible(killsReport, 1);
    if (overall)
        MissionInfo_SetVisible(overall, 1);

    void* radio = MissionInfo_FindControl(
        MissionInfo_Pointer(self->field_001c), CSF_STR(0x00842EC8, "GRadio"));
    void* mapTab = MissionInfo_FindControl(
        radio, CSF_STR(0x00842EAC, "BotonPestaniaMapaTactico"));
    void* objectivesTab = MissionInfo_FindControl(
        radio, CSF_STR(0x00842E94, "BotonPestaniaObjetivos"));

    if (CSF_AT(uint8_t, MissionInfo_Pointer(self->field_0014), 0x4))
    {
        if (self->field_0084)
            MissionInfo_SetVisible(MissionInfo_Pointer(self->field_0084), 0);
        if (tacticalMap)
            MissionInfo_SetVisible(tacticalMap, 0);
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, radio, 0)[0x270 / 4])(radio, nullptr, mapTab);
    }
    else
    {
        if (objectives)
            MissionInfo_SetVisible(objectives, 0);
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, radio, 0)[0x270 / 4])(radio, nullptr, objectivesTab);
    }

    CMenuOptions_ApplyTargetOffset(reinterpret_cast<CMenuOptions*>(self), nullptr);
    if (self->field_0014)
    {
        if (self->field_0020)
            MissionInfo_SetVisible(MissionInfo_Pointer(self->field_0020), 1);
        if (self->field_002c)
            MissionInfo_SetVisible(MissionInfo_Pointer(self->field_002c), 1);
        if (self->field_0024)
            MissionInfo_SetVisible(MissionInfo_Pointer(self->field_0024), 1);
        if (self->field_0028)
            MissionInfo_SetVisible(MissionInfo_Pointer(self->field_0028), 1);

        if (!CSF_AT(uint8_t, MissionInfo_Pointer(self->field_0014), 0x4))
        {
            uint32_t hiddenButton;
            if (MissionInfo_HasNormalDestination() &&
                CSF_AT(uint8_t, MissionInfo_Pointer(GetGlobalValue_75FDF0()), 241))
            {
                char* missionName = reinterpret_cast<char*>(
                    S_00500350_GetFirstRecordNameBuffer(
                        reinterpret_cast<S_00500350*>(GetGlobalValue_8B0568()),
                        nullptr));
                int mission = Campaign_FindMission(
                    MissionInfo_Pointer(GetGlobal8afc7c()), nullptr, missionName);
                hiddenButton =
                    mission && CSF_AT(uint8_t, MissionInfo_Pointer(mission), 192)
                    ? self->field_002c : self->field_0020;
            }
            else
            {
                hiddenButton = !CSF_AT(
                    uint8_t, MissionInfo_Pointer(GetGlobalValue_75FDF0()), 204)
                    ? self->field_0028 : self->field_0024;
            }
            if (hiddenButton)
                MissionInfo_SetVisible(MissionInfo_Pointer(hiddenButton), 0);
        }
    }

    void* stateObject =
        CSF_AT(void*, MissionInfo_Pointer(GetGlobalState_50D190()), 0x10);
    if (stateObject)
    {
        int settings = GetGlobalValue_753A30();
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, stateObject, 0)[0x20 / 4])(
                stateObject, nullptr, CSF_AT(int, MissionInfo_Pointer(settings), 512));
    }

    Get_008b0678();
    MenuMissionInfo_SetGlobalIndex(96);

    if (MissionInfo_FindControl(
            MissionInfo_Pointer(self->field_001c),
            CSF_STR(0x00842E84, "BalaSuperpuesta")))
    {
        void* overlay = MissionInfo_FindControl(
            MissionInfo_Pointer(self->field_001c),
            CSF_STR(0x00842E84, "BalaSuperpuesta"));
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, overlay, 0)[0xC8 / 4])(overlay, nullptr, 1);
    }
    if (MissionInfo_FindControl(
            MissionInfo_Pointer(self->field_001c),
            CSF_STR(0x00842E70, "PistolaSuperpuesta")))
    {
        void* overlay = MissionInfo_FindControl(
            MissionInfo_Pointer(self->field_001c),
            CSF_STR(0x00842E70, "PistolaSuperpuesta"));
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, overlay, 0)[0xC8 / 4])(overlay, nullptr, 1);
    }
    if (MissionInfo_FindControl(
            MissionInfo_Pointer(self->field_001c),
            CSF_STR(0x00842E5C, "CadenaSuperpuesta")))
    {
        void* overlay = MissionInfo_FindControl(
            MissionInfo_Pointer(self->field_001c),
            CSF_STR(0x00842E5C, "CadenaSuperpuesta"));
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, overlay, 0)[0xC8 / 4])(overlay, nullptr, 1);
    }

    void* screenObject = MissionInfo_Pointer(self->field_0010);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, screenObject, 0)[0xA8 / 4])(screenObject, nullptr);

    void* destination = UmsVirtualProcessor_GetDefaultDestination(
        MissionInfo_Pointer(GetGlobalValue_8B0568()), nullptr);
    if (CSF_AT(int, destination, 0x1C) != -1)
    {
        C_00840fdc_ClearSceneResources(
            reinterpret_cast<C_00840fdc*>(GetGlobalValue_51F740()), nullptr, 1);
        GetGlobalInt();
        MissionInfo_PurgeRenderResources();
        RwFreeListPurgeAllFreeLists();
    }

    if (!CSF_AT(uint8_t, MissionInfo_Pointer(self->field_0014), 0x4))
    {
        void* profile =
            CSF_AT(void*, MissionInfo_Pointer(GetGlobalValue_8B056C()), 0x8);
        void* scenarios = ProfileSavegames_GetScenarioCollection(profile, nullptr);
        uint32_t first = CSF_AT(uint32_t, scenarios, 0x4);
        char hasSavedScenario = 0;
        if (first &&
            static_cast<int32_t>(CSF_AT(uint32_t, scenarios, 0x8) - first) / 80)
        {
            int scenario =
                reinterpret_cast<int (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, profile, 0)[0x24 / 4])(profile, nullptr);
            hasSavedScenario =
                CMenuMissionInfo_CheckSavedScenario(self, nullptr, scenario);
        }
        self->field_00a7 = hasSavedScenario;

        if (CSF_AT(uint8_t, MissionInfo_Pointer(GetGlobalValue_75FDF0()), 209) &&
            MissionInfo_HasNormalDestination() &&
            !MissionInfo_IsNextMissionUnavailable())
        {
            self->field_00a8 = 1;
            return self->field_001c;
        }
        self->field_00a8 = 0;
    }
    return self->field_001c;
}

HOOK(0x0053ECD0, CMenuMissionInfo_ShowMissionReport, "auto");
