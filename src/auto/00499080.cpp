#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CCharActionFsm_InvokeVirtual308, 0x004835A0, (CCharActionFsm* self, void*));
CSF_FUNC(void, __fastcall, Hud_ClearPendingAction, 0x00497750, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, InvokeMemberVirtualSlot8, 0x004F5170, (void* self, void* edx, float angleRadians));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, C_00833908_SetSceneResourceIfSceneMatches, 0x00525A50, (C_00833908* self, void*, int sceneId, int resourceId));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(float, g_DegreesToRadians, 0x00833DA0);

static int __fastcall C_00833908_ResetActionStateAndRestoreView(C_00833908* self, void* /*edx*/)
{
    CCharActionFsm_InvokeVirtual308(reinterpret_cast<CCharActionFsm*>(self->field_0044), nullptr);

    if ((CSF_AT(uint8_t, self, 0x2E0) & 0x40) != 0)
    {
        Hud_ClearPendingAction(self, nullptr);

        // Default angle (degrees) from the global settings object.
        float angleRadians = CSF_AT(float, GetGlobalValue_753A30(), 0x13C) * g_DegreesToRadians;

        // An override angle of -1.0f means "not set"; otherwise it wins.
        int overrideOwner = GetValue8B0554();
        if (CSF_AT(int32_t, overrideOwner, 0x90C) != (int32_t)0xBF800000)
        {
            angleRadians = CSF_AT(float, overrideOwner, 0x90C) * g_DegreesToRadians;
        }

        int container = GetGlobalValue_4FBDE0();
        int firstElement = GetFirstElement(reinterpret_cast<void*>(container), nullptr);
        void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
        InvokeMemberVirtualSlot8(provider, nullptr, angleRadians);

        CSF_AT(float, self, 0x390) = 1.0f;
    }

    int sceneManager = Get_008b0678();
    int result = C_00833908_SetSceneResourceIfSceneMatches(
        reinterpret_cast<C_00833908*>(sceneManager), nullptr, reinterpret_cast<int>(self), -1);

    CSF_AT(uint32_t, self, 0x2E0) = 0x8000;
    CSF_AT(uint32_t, self, 0x2E4) = 0;
    return result;
}

HOOK(0x00499080, C_00833908_ResetActionStateAndRestoreView, "auto");
