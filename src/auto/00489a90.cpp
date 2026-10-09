#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, CharActionFsm_CreateAction, 0x00475A40,
         (int a1, int a2, int a3));
CSF_FUNC(void, __fastcall, C_00862490_SetActive, 0x00476170,
         (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, CCharActionFsm_AddAction, 0x004892B0,
         (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, CCharActionFsm_ActivateSubState, 0x00489470,
         (void* self, void* edx, int a2, void* a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, IsSubStateEnabled, 0x00760F90, (int a1));

static void __fastcall CCharActionFsm_ActivateAction(
    CCharActionFsm* self, void* /*edx*/, int actionId, void* context)
{
    const int index = self->vFindProviderIndex(actionId);
    if (index != -1)
    {
        void* action = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0018), index * 4);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, action, 0)[0x5C / 4])(
                action, nullptr, context);
        return;
    }

    void* globals = reinterpret_cast<void*>(GetValue8B0554());
    void* definitions = CSF_AT(void*, globals, 0x1FC0);
    void* definition =
        reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, definitions, 0)[8 / 4])(
                definitions, nullptr, actionId);

    if (static_cast<uint8_t>(
            IsSubStateEnabled(CSF_AT(int, definition, 0x74))))
    {
        CCharActionFsm_ActivateSubState(self, nullptr, actionId, context);
        return;
    }

    uint32_t* action = CharActionFsm_CreateAction(
        1, actionId, static_cast<int>(self->field_0008));
    CCharActionFsm_AddAction(self, nullptr, action);
    C_00862490_SetActive(action, nullptr, reinterpret_cast<void*>(1));

    if (self->field_0024 == static_cast<uint32_t>(-1))
    {
        void* lastAction = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_001c - 4), 0);
        const bool canActivate =
            reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, lastAction, 0)[0x98 / 4])(
                    lastAction, nullptr);

        if (canActivate)
        {
            const uint32_t begin = self->field_0018;
            if (begin)
            {
                self->vSetCurrentWeaponAction(
                    (static_cast<int32_t>(self->field_001c - begin) >> 2) - 1);
            }
            else
            {
                self->vSetCurrentWeaponAction(-1);
            }
        }
    }
}

HOOK(0x00489A90, CCharActionFsm_ActivateAction, "auto");
