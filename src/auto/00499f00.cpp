#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(void, __fastcall, C_42def0_ClearByteFlag, 0x0042DEF0, (void* self, void*));
CSF_FUNC(int, __cdecl, MissionInfo_IsPrerequisiteMet, 0x0044FB00, ());
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_DispatchWithAimAngle, 0x00483540, (CCharActionFsm* self, void* , float a2));
CSF_FUNC(int, __fastcall, CCharActionFsm_UpdateFlagsAndDispatch, 0x00484910, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_UpdateWeaponActionState, 0x004849A0, (CCharActionFsm* self, void*));
CSF_FUNC(void, __fastcall, Hud_ClearPendingAction, 0x00497750, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, SelectionManager_Refresh, 0x004FBFD0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SelectionManager_SetMode, 0x004FC210, (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, GlobalValue_SetSelection, 0x004FC3B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, CSoundManager_SetVolumeModeFlag, 0x00522B40, (CSoundManager* self, void* , uint32_t flagArg));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, C_00833908_SetSceneResourceIfSceneMatches, 0x00525A50, (C_00833908* self, void* , int sceneId, int resourceId));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resets the HUD/aim state of the active scene menu context: restores time scale,
// clears pending actions and selection, frees the two temporary buffers and clears state flags.
static int __fastcall C_00833b88_ResetActionState(C_00833b88* self, void* /*edx*/, char keepWeaponState)
{
    int result = MissionInfo_IsPrerequisiteMet();
    if ((char)result == 0)
        return result;

    result = (int)GetSceneMenuContext(g_pCurrentScene, nullptr);
    if ((uint32_t)(uintptr_t)self != (uint32_t)result)
        return result;

    CCharActionFsm* actionFsm = reinterpret_cast<CCharActionFsm*>(self->field_0044);

    self->vSlot70(0x3F800000);

    if (CSF_AT(uint8_t, GetGlobalState_50D190(), 0xD0) == 0)
    {
        CSoundManager_SetVolumeModeFlag(
            reinterpret_cast<CSoundManager*>(GetGlobalValue_521BB0()), nullptr, 0);
    }

    if ((CSF_AT(uint8_t, self, 0x2E0) & 0x40) != 0)
        Hud_ClearPendingAction(self, nullptr);

    if ((CSF_AT(uint32_t, self, 0x2E0) & 0x40000) != 0)
        CCharActionFsm_UpdateFlagsAndDispatch(actionFsm, nullptr);

    CCharActionFsm_DispatchWithAimAngle(actionFsm, nullptr, 0.0f);

    CSF_AT(float, self, 0x390) = 1.0f;

    SelectionManager_SetMode(reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr, 0);
    SelectionManager_Refresh(reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr);

    C_00833908_SetSceneResourceIfSceneMatches(
        reinterpret_cast<C_00833908*>(Get_008b0678()), nullptr, (int)(uintptr_t)self, -1);

    CSF_AT(uint32_t, GetGlobal8afc78(), 0x234) = 0;
    C_42def0_ClearByteFlag(reinterpret_cast<void*>(GetGlobal8afc78()), nullptr);

    GlobalValue_SetSelection(reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr, -1);

    if (CSF_AT(void*, self, 0x300) != nullptr)
        Mem_FreeDebug(CSF_AT(void*, self, 0x300));
    CSF_AT(uint32_t, self, 0x300) = 0;
    CSF_AT(uint32_t, self, 0x304) = 0;
    CSF_AT(uint32_t, self, 0x308) = 0;

    if (CSF_AT(void*, self, 0x310) != nullptr)
        Mem_FreeDebug(CSF_AT(void*, self, 0x310));
    CSF_AT(uint32_t, self, 0x310) = 0;
    CSF_AT(uint32_t, self, 0x314) = 0;
    CSF_AT(uint32_t, self, 0x318) = 0;

    uint32_t activeFlags = CSF_AT(uint32_t, self, 0x2E0) & 0xFFF9A9AC;
    uint32_t pendingFlags = CSF_AT(uint32_t, self, 0x2E4) & 0xFFF9A9AC;
    CSF_AT(uint32_t, self, 0x2E0) = activeFlags;
    CSF_AT(uint32_t, self, 0x2E4) = pendingFlags;
    result = (int)pendingFlags;

    if (keepWeaponState == 0)
    {
        CSF_AT(uint32_t, self, 0x2E0) = activeFlags & 0xFFFFFE7F;
        CSF_AT(uint32_t, self, 0x2E4) = pendingFlags & 0xFFFFFE7F;
        return CCharActionFsm_UpdateWeaponActionState(actionFsm, nullptr);
    }

    return result;
}

HOOK(0x00499F00, C_00833b88_ResetActionState, "auto");
