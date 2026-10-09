#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Bindings for callees of the original (by address)
CSF_FUNC(int, __fastcall, CharActionFsm_GetCurrentState, 0x004834F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_OnDefaultState, 0x00480140, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CCharActionFsm_ClearSubState, 0x004847C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_GetActiveSubState, 0x00487260, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_HandleState10, 0x00487C40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));
CSF_FUNC(int, __fastcall, Hud_SetSuspended, 0x004954F0, (void* self, void* edx, char a1));
CSF_FUNC(char, __fastcall, Hud_IsSuspended, 0x00495520, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Hud_ClearPendingAction, 0x00497750, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Hud_SetDisplayFlags, 0x00498F50, (void* self, void* edx, int a2, char a3));

using VirtualNoArgFn = int(__fastcall*)(void* self, void* edx);
using VirtualCallSlot0Fn = int(__fastcall*)(void* self, void* edx);

// Indirect tail path of case 0: forwards ecx=this to vtbl[0x144/4] (vSlot144) unchanged.
static __declspec(naked) void CCharActionFsm_TailVSlot144()
{
    __asm {
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0x144]
    }
}

static int __fastcall CCharActionFsm_DispatchActionState(CCharActionFsm* self, void* /*edx*/)
{
    int result = 0;
    uint32_t stateMode = self->m_stateCode;

    switch (stateMode)
    {
    case 0:
        // Original tail-jumps through vtbl[0x144/4]; forwarded via the naked thunk.
        result = reinterpret_cast<VirtualCallSlot0Fn>(&CCharActionFsm_TailVSlot144)(self, nullptr);
        break;

    case 10:
        result = CCharActionFsm_HandleState10(self, nullptr);
        break;

    case 11:
    {
        Hud_SetDisplayFlags(reinterpret_cast<void*>(self->field_0008), nullptr, 1024, 1);
        uint32_t pendingSubState = self->field_009c;
        result = static_cast<int>(pendingSubState);
        if (pendingSubState != 0)
        {
            void* pendingObj = reinterpret_cast<void*>(pendingSubState);
            result = (CSF_AT(VirtualNoArgFn*, pendingObj, 0))[0x4C / 4](pendingObj, nullptr);
            if (result >= 10 && result <= 12)
                result = self->vUpdateWeaponSelection(12);
        }
        break;
    }

    case 12:
    {
        Hud_SetDisplayFlags(reinterpret_cast<void*>(self->field_0008), nullptr, 256, 1);

        if (self->field_00a4 == 0)
        {
            void* newSubState = nullptr;
            if (self->field_0084 != 0 && Hud_IsSuspended(reinterpret_cast<void*>(self->field_0008), nullptr))
            {
                newSubState = reinterpret_cast<void*>(self->field_0084);
            }
            else
            {
                uint32_t subStateIndex = self->field_0090;
                if (subStateIndex != 0xFFFFFFFFu)
                    newSubState = *reinterpret_cast<void**>(self->field_0078 + 4 * subStateIndex);
            }
            self->field_00a4 = reinterpret_cast<uint32_t>(newSubState);
        }

        void* currentSub = reinterpret_cast<void*>(self->field_00a4);
        int subResult = (CSF_AT(VirtualNoArgFn*, currentSub, 0))[0x4C / 4](currentSub, nullptr);

        switch (subResult)
        {
        case 3:
        case 10:
        case 11:
            if (self->field_009c != 0)
                return self->vSetStateAndDispatch(13);
            return self->vSetStateAndDispatch(12);

        case 12:
            if ((CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x1F0) & 2) != 0)
                self->field_009c = self->field_00a4;
            return self->vSetStateAndDispatch(12);

        case 13:
        case 14:
            if (self->field_009c == 0)
                return self->vSetStateAndDispatch(12);
            {
                int currentState = CharActionFsm_GetCurrentState(self, nullptr);
                if (currentState < 10 || currentState > 11)
                    return self->vSetStateAndDispatch(13);
                return self->vSetStateAndDispatch(15);
            }

        case 15:
        {
            int activeSubState = CCharActionFsm_GetActiveSubState(self, nullptr);
            if (activeSubState != 0 && CSF_AT(int, reinterpret_cast<void*>(activeSubState), 0x80) != 0)
            {
                if (C_004950e0_GetField398(reinterpret_cast<void*>(self->field_0008), nullptr))
                    return self->vSetStateAndDispatch(22);
                return self->vSetStateAndDispatch(20);
            }
            CCharActionFsm_ClearSubState(self, nullptr);
            return self->vBeginWeaponStateChange(0, 0);
        }

        default:
            CCharActionFsm_ClearSubState(self, nullptr);
            return self->vBeginWeaponStateChange(0, 0);
        }
    }

    case 13:
    {
        void* hud = reinterpret_cast<void*>(self->field_0008);
        Hud_SetDisplayFlags(hud, nullptr, 64, 1);
        if (Hud_IsSuspended(reinterpret_cast<void*>(self->field_0008), nullptr))
            Hud_SetSuspended(reinterpret_cast<void*>(self->field_0008), nullptr, 1);
        result = self->vSetStateAndDispatch(23);
        break;
    }

    case 14:
    {
        Hud_ClearPendingAction(reinterpret_cast<void*>(self->field_0008), nullptr);
        Hud_SetDisplayFlags(reinterpret_cast<void*>(self->field_0008), nullptr, 64, 0);
        void* hud = reinterpret_cast<void*>(self->field_0008);
        if ((CSF_AT(uint8_t, hud, 0x2E0) & 1) != 0)
            CSF_AT(uint32_t, hud, 0x2E4) &= 0xFFFFFFBFu;
        result = self->vSetStateAndDispatch(24);
        break;
    }

    case 15:
        if ((CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x2E0) & 0x40) != 0)
            result = self->vSetStateAndDispatch(25);
        else
            result = self->vSetStateAndDispatch(26);
        break;

    case 16:
        result = static_cast<int>(stateMode);
        break;

    case 17:
        result = self->vSetStateAndDispatch(28);
        break;

    case 18:
        result = self->vSetStateAndDispatch(30);
        break;

    default:
        result = CCharActionFsm_OnDefaultState(self, nullptr);
        break;
    }

    return result;
}

HOOK(0x00488980, CCharActionFsm_DispatchActionState, "auto");
