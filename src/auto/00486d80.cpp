#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_unknown_GetLinkedDword, 0x00476080, (void* self, void*));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(void, __fastcall, CCharWeaponFsm_UpdateWeapon, 0x00480B60, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CCharActionFsm_UpdateAction, 0x00485870, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_495650_SetChildField20ToHalf, 0x00495650, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, CharActionFsm_CheckSubObjectState, 0x00761330, (int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CCharActionFsm_UpdateAndNotifyScene(
    CCharActionFsm* self, void* /*edx*/)
{
    ++self->field_00c0;
    C_495650_SetChildField20ToHalf(
        reinterpret_cast<void*>(self->field_0008), nullptr);
    CCharActionFsm_UpdateAction(self, nullptr);
    CCharWeaponFsm_UpdateWeapon(self, nullptr);

    int result = static_cast<int>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (static_cast<uint32_t>(result) != self->field_0008)
        return result;

    auto* subObject = reinterpret_cast<C_008319c0*>(self->vGetIndexedEntryOrZero());
    int subObjectState = C_008319c0_GetSubObjectField74(subObject, nullptr);
    result = CharActionFsm_CheckSubObjectState(subObjectState);
    if (static_cast<uint8_t>(result) == 0)
        return result;

    result = GetGlobalState_50D190();
    void* notificationTarget = CSF_AT(void*, reinterpret_cast<void*>(result), 0x74);
    if (!notificationTarget)
        return result;

    float notificationData[4] = {};

    void* handle = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
    void* value = reinterpret_cast<void*>(Handle_GetValuePointer(handle, nullptr));
    notificationData[3] = static_cast<float>(CSF_AT(int, value, 0x118));

    void* linkedObject = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
    int linkedType = C_unknown_GetLinkedDword(linkedObject, nullptr);

    if (linkedType == 2)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int, float*)>(
            CSF_AT(void**, notificationTarget, 0)[1])(
                notificationTarget, nullptr, 2, notificationData);
        self->field_00d0 = 0.11f;
        return result;
    }

    return reinterpret_cast<int (__fastcall*)(void*, void*, int, float*)>(
        CSF_AT(void**, notificationTarget, 0)[1])(
            notificationTarget, nullptr, 1, notificationData);
}

HOOK(0x00486D80, CCharActionFsm_UpdateAndNotifyScene, "auto");
