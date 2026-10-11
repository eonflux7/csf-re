#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, CCharActionFsm_IsSubObjectCounterNonNegative, 0x00483FF0, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_IsSubFsmModeThree, 0x00484010, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_IsStateCodeFortyOrFortyOne, 0x00484030, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_IsSubFsmModeFourteen, 0x00484050, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_ResolveTargetFromSlot, 0x00485960, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(char, __fastcall, HudAlertIcons_PrepareCapture, 0x004957C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00833b88_ReleaseCapture, 0x0049AA20, (C_00833b88* self, void*));
CSF_FUNC(int, __fastcall, HudAlertIcons_CheckActionAlert, 0x004A19C0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, HudAlertIcons_UpdateAssociatedState, 0x004A1C80, (int a1));
CSF_FUNC(int, __fastcall, HudAlertIcons_SetAssociatedMode, 0x004A2290, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(bool, __fastcall, MpSpawnPoints_IsSpawnChangeAllowed, 0x004F5740, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(bool, __fastcall, SpawnPoints_IsSabotageSpawnReady, 0x00551530, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CMenuMpStats_SetFlagByte1088IfReady, 0x00740970, (CMenuMpStats* self, void* , char newValue));
CSF_FUNC(char, __fastcall, CMenuMpStats_SetFlagByte1103IfReady, 0x007409A0, (CMenuMpStats* self, void* , char newValue));
CSF_FUNC(char, __fastcall, NetCogibleDDL_ApplyRotationSnapshot, 0x007409D0, (void* self, void* , float a3, float a4, float a5));
CSF_FUNC(char, __fastcall, CMenuMpStats_ProcessStatsUpdate, 0x00740A60, (CMenuMpStats* self, void*));
CSF_FUNC(char, __fastcall, CMenuMpStats_StoreValueIfReady, 0x00740A80, (CMenuMpStats* self, void* , int16_t value));
CSF_FUNC(int, __fastcall, NetCogibleDdl_CopyFlagAndCheckActive, 0x00740DA0, (void* self, void* , uint8_t* outFlag));
CSF_FUNC(int, __fastcall, CMenuMpStats_GetPlayerExclusionFlag, 0x007411A0, (CMenuMpStats* self, void*));
CSF_FUNC(void, __fastcall, CMenuMpStats_UpdateAimDirection, 0x00741460, (void* self, void* , int a2, int a3, int a4));

static void __fastcall HudAlertIcons_FinishAssociatedUpdate(
    void* self, void* /*edx*/, float elapsed)
{
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        return;

    void* association = CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x18);
    CMenuMpStats* originalStats = association
        ? reinterpret_cast<CMenuMpStats*>(static_cast<uint8_t*>(association) - 1712)
        : nullptr;

    alignas(8) uint8_t flagBytes[0x2];
    flagBytes[1] = association
        ? static_cast<uint8_t>(
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, association, 0)[0x8 / 4])(association, nullptr))
        : 0;

    if (originalStats)
    {
        if (flagBytes[1])
        {
            association = CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x18);
            CMenuMpStats* stats = association
                ? reinterpret_cast<CMenuMpStats*>(static_cast<uint8_t*>(association) - 1712)
                : nullptr;

            HudAlertIcons_UpdateAssociatedState(reinterpret_cast<int>(stats));

            if (CSF_AT(uint8_t, self, 0x4))
            {
                // The original also takes the subtraction branch for an unordered comparison.
                if (!(CSF_AT(float, self, 0) <= g_fZero))
                    CSF_AT(float, self, 0) = CSF_AT(float, self, 0) - elapsed;
                else
                    HudAlertIcons_SetAssociatedMode(self, nullptr, stats, 0);
            }
            else
            {
                flagBytes[0] = 0;
                if (static_cast<uint8_t>(
                        NetCogibleDdl_CopyFlagAndCheckActive(stats, nullptr, flagBytes)))
                {
                    if (flagBytes[0])
                        HudAlertIcons_SetAssociatedMode(self, nullptr, stats, 1);
                    else
                    {
                        CSF_AT(uint8_t, self, 0x4) = 1;
                        CSF_AT(float, self, 0) = 5.0f;
                    }
                }
                else
                {
                    void* firstPosition = reinterpret_cast<void*>(
                        GetObjectPositionVector(CSF_AT(void*, self, 0x8), nullptr));
                    void* secondPosition = reinterpret_cast<void*>(
                        GetObjectPositionVector(CSF_AT(void*, self, 0x8), nullptr));
                    int positionZ = CSF_AT(int, secondPosition, 0x8);
                    void* thirdPosition = reinterpret_cast<void*>(
                        GetObjectPositionVector(CSF_AT(void*, self, 0x8), nullptr));
                    int positionY = CSF_AT(int, thirdPosition, 0x4);
                    int positionX = CSF_AT(int, firstPosition, 0);
                    CMenuMpStats_UpdateAimDirection(
                        stats, nullptr, positionX, positionY, positionZ);

                    void* object = CSF_AT(void*, self, 0x8);
                    float rotation = CSF_AT(float, object, 0x1B0);
                    float scalar = static_cast<float>(
                        C_008342e8_ComputeResolvedObjectScalar(
                            static_cast<C_008342e8*>(object), nullptr));
                    NetCogibleDDL_ApplyRotationSnapshot(
                        stats, nullptr, scalar, rotation, 0.0f);

                    CCharActionFsm* action = CSF_AT(
                        CCharActionFsm*, CSF_AT(void*, self, 0x8), 0x44);
                    int secondarySlot = action->vGetSecondaryCount();
                    int targetValue = -1;
                    if (CCharActionFsm_ResolveTargetFromSlot(action, nullptr))
                    {
                        void* target = reinterpret_cast<void*>(
                            CCharActionFsm_ResolveTargetFromSlot(action, nullptr));
                        targetValue = CSF_AT(
                            int, reinterpret_cast<void*>(
                                Handle_GetValuePointer(target, nullptr)), 0);
                    }

                    if (secondarySlot == -1)
                    {
                        CMenuMpStats_SetFlagByte1088IfReady(stats, nullptr, 0);
                    }
                    else
                    {
                        void* entry = reinterpret_cast<void*>(
                            action->vGetIndexedEntry(secondarySlot));
                        int entryValue = CSF_AT(
                            int, reinterpret_cast<void*>(
                                Handle_GetValuePointer(entry, nullptr)), 0);
                        if (entryValue != -1)
                            CMenuMpStats_SetFlagByte1088IfReady(
                                stats, nullptr, static_cast<char>(entryValue));
                    }
                    if (targetValue != -1)
                        CMenuMpStats_SetFlagByte1103IfReady(
                            stats, nullptr, static_cast<char>(targetValue));

                    if (CSF_AT(uint8_t, self, 0x10))
                    {
                        CMenuMpStats_ProcessStatsUpdate(stats, nullptr);
                        CSF_AT(uint8_t, self, 0x10) = 0;
                    }

                    uint16_t alertFlags = 0;
                    void* controller = CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x38);
                    int controllerValue =
                        reinterpret_cast<int(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, controller, 0)[0xA0 / 4])(
                                controller, nullptr);

                    if (static_cast<uint8_t>(
                            SceneObj_IsInactive(CSF_AT(void*, self, 0x8), nullptr)))
                    {
                        alertFlags = 64;
                    }
                    else if (static_cast<uint8_t>(
                                 SceneObject_PassesFirstCheck(
                                     CSF_AT(void*, self, 0x8), nullptr)))
                    {
                        alertFlags = 256;
                    }
                    else
                    {
                        if (controllerValue != CSF_AT(int, self, 0xC))
                        {
                            alertFlags = 32;
                            CSF_AT(int, self, 0xC) = controllerValue;
                        }

                        uint32_t objectFlags =
                            CSF_AT(uint32_t, CSF_AT(void*, self, 0x8), 0x2E0);
                        if ((objectFlags & 0x80u) ||
                            static_cast<uint8_t>(
                                HudAlertIcons_CheckActionAlert(action, nullptr)))
                        {
                            alertFlags |= 0x200u;
                        }
                        else if (CSF_AT(float, self, 0x20) > g_fZero)
                        {
                            alertFlags |= 0x80u;
                        }
                        else if (objectFlags & 0x100)
                        {
                            alertFlags |= 0x400u;
                        }

                        if (objectFlags & 8)
                            alertFlags |= 1u;
                        else if (objectFlags & 4)
                            alertFlags |= 8u;

                        if (CCharActionFsm_IsSubObjectCounterNonNegative(action, nullptr))
                            alertFlags |= 0x800u;
                        if (static_cast<uint8_t>(
                                CCharActionFsm_IsSubFsmModeThree(action, nullptr)))
                            alertFlags |= 0x1000u;
                        if (static_cast<uint8_t>(
                                CCharActionFsm_IsSubFsmModeFourteen(action, nullptr)))
                            alertFlags |= 0x2000u;
                        if (CSF_AT(uint8_t, CSF_AT(void*, self, 0x8), 0x1F0) & 2)
                            alertFlags |= 0x4000u;
                        if (static_cast<uint8_t>(
                                CCharActionFsm_IsStateCodeFortyOrFortyOne(action, nullptr)))
                            alertFlags |= 0x8000u;
                    }

                    CMenuMpStats_StoreValueIfReady(
                        stats, nullptr, static_cast<int16_t>(alertFlags));
                }
            }
        }

        if (!CSF_AT(uint8_t, CSF_AT(void*, self, 0x8), 0x3A4) &&
            (SpawnPoints_IsSabotageSpawnReady(
                 reinterpret_cast<void*>(GetGlobal8B0688()), nullptr) ||
             !CSF_AT(uint8_t, originalStats, 0x443) ||
             (static_cast<uint8_t>(
                  CMenuMpStats_GetPlayerExclusionFlag(originalStats, nullptr)) &&
              MpSpawnPoints_IsSpawnChangeAllowed(
                  GetCachedObjectProvider(
                      reinterpret_cast<void*>(
                          GetFirstElement(
                              reinterpret_cast<void*>(GetGlobalValue_4FBDE0()),
                              nullptr)),
                      nullptr),
                  nullptr))))
        {
            if (flagBytes[1])
            {
                HudAlertIcons_PrepareCapture(CSF_AT(void*, self, 0x8), nullptr);
                void* controller = CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x38);
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, controller, 0)[0xA4 / 4])(
                        controller, nullptr, 0);
            }
            else
            {
                CSF_AT(uint8_t, CSF_AT(void*, self, 0x8), 0x3A4) = 1;
            }
        }
        else if (CSF_AT(uint8_t, CSF_AT(void*, self, 0x8), 0x3A4))
        {
            if (!SpawnPoints_IsSabotageSpawnReady(
                    reinterpret_cast<void*>(GetGlobal8B0688()), nullptr) &&
                !static_cast<uint8_t>(
                    CMenuMpStats_GetPlayerExclusionFlag(originalStats, nullptr)) &&
                CSF_AT(uint8_t, originalStats, 0x443))
            {
                if (flagBytes[1])
                    C_00833b88_ReleaseCapture(
                        CSF_AT(C_00833b88*, self, 0x8), nullptr);
                else
                    CSF_AT(uint8_t, CSF_AT(void*, self, 0x8), 0x3A4) = 0;
            }
        }
    }

    if (CSF_AT(float, self, 0x20) >= g_fZero)
        CSF_AT(float, self, 0x20) = CSF_AT(float, self, 0x20) - elapsed;
}

HOOK(0x004A2490, HudAlertIcons_FinishAssociatedUpdate, "auto");
