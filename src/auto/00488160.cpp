#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee bindings (addresses from the packet, signatures as given).
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_4762C0, 0x004762C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_480110, 0x00480110, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_482740, 0x00482740, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_482780, 0x00482780, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4829B0, 0x004829B0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_4846D0, 0x004846D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_484CF0, 0x00484CF0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_486460, 0x00486460, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_486980, 0x00486980, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_486A60, 0x00486A60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_498F50, 0x00498F50, (void* self, void* edx, int a2, char a3));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Clears one bit in the weapon-state word (field_0008 + 0x2E4) and notifies 0x498F50 with that mask.
static int CCharActionFsm_ClearActionFlag(CCharActionFsm* self, uint32_t flagMask) {
    void* weaponState = reinterpret_cast<void*>(self->field_0008);
    uint32_t& flags = CSF_AT(uint32_t, weaponState, 0x2E4);
    flags &= ~flagMask;
    return sub_498F50(weaponState, nullptr, static_cast<int>(flagMask), 0);
}

// Tail of the inner-switch paths: resets the pending entry and clears flag 0x100.
static int CCharActionFsm_CloseInnerState(CCharActionFsm* self) {
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    return CCharActionFsm_ClearActionFlag(self, 0x100u);
}

// Aim-target resolution used when field_0010 == 0xE.
static void CCharActionFsm_ResolveAimTarget(CCharActionFsm* self) {
    if (self->field_009c != 0) {
        void* target = reinterpret_cast<void*>(self->field_009c);
        int targetState = ((int(__fastcall*)(void*, void*))(CSF_AT(void**, target, 0)[19]))(target, nullptr);
        if (targetState >= 10 && targetState <= 11) {
            sub_4829B0(self, nullptr);
            return;
        }
    }
    if (self->field_009c != 0 &&
        (sub_4762C0(reinterpret_cast<void*>(self->field_009c), nullptr) & 0xFF) != 0) {
        sub_4846D0(self, nullptr);
        sub_4829B0(self, nullptr);
    }
}

// Leaves the current action state; returns the low byte the original returns in al.
static char __fastcall CCharActionFsm_ExitCurrentState(CCharActionFsm* self, void* /*edx*/, int contextArg) {
    int result = static_cast<int>(self->m_stateCode);

    switch (self->m_stateCode) {
    case 2u:
        result = CCharActionFsm_ClearActionFlag(self, 0x200u);
        break;
    case 3u:
        result = sub_486980(self, nullptr, contextArg);
        break;
    case 4u:
        result = CCharActionFsm_ClearActionFlag(self, 0x80u);
        self->field_0070 = 0;
        break;
    case 5u:
    case 0xFu:
        result = sub_482740(self, nullptr, contextArg);
        break;
    case 7u:
        result = CCharActionFsm_ClearActionFlag(self, 0x10u);
        break;
    case 0xAu:
        result = sub_486A60(self, nullptr, contextArg);
        break;
    case 0xBu:
        result = CCharActionFsm_ClearActionFlag(self, 0x400u);
        break;
    case 0xCu:
        switch (self->field_0010) {
        case 0xDu:
            sub_4846D0(self, nullptr);
            sub_4829B0(self, nullptr);
            break;
        case 0xEu:
            CCharActionFsm_ResolveAimTarget(self);
            break;
        case 0x11u:
        case 0x12u:
            sub_486460(self, nullptr);
            break;
        case 0x15u:
            sub_484CF0(self, nullptr);
            break;
        default:
            break;
        }
        result = CCharActionFsm_CloseInnerState(self);
        break;
    case 0x10u:
        result = sub_482780(self, nullptr, contextArg);
        break;
    case 0x11u: {
        result = static_cast<int>(self->field_00b8);
        if (result >= 0) {
            CCharActionFsm_ClearActionFlag(self, 0x200u);
            int handleValue = static_cast<int>(reinterpret_cast<uintptr_t>(self) + 0xB0u);
            result = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleValue);
            if (result != 0) {
                void* resolvedObj = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
                result = ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, resolvedObj, 0)[26]))(
                    resolvedObj, nullptr, 1);
            }
            CSF_AT(int, self, 0xB4) = -1;
            self->field_00b8 = 0xFFFFFFFFu;
        }
        if (self->field_0024 == 0xFFFFFFFFu) {
            uint32_t elementStart = self->field_0018;
            if (elementStart != 0) {
                result = static_cast<int>(self->field_001c - elementStart) >> 2;
                if (result != 0) {
                    self->field_0024 = 0;
                    int childValue = self->vGetIndexedEntryOrZero();
                    void* childObj = reinterpret_cast<void*>(static_cast<uintptr_t>(childValue));
                    result = ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, childObj, 0)[14]))(
                        childObj, nullptr, 0);
                }
            }
        }
        break;
    }
    case 0x12u:
        if (self->field_0028 != 0xFFFFFFFFu) {
            self->vSetCurrentWeaponAction(static_cast<int>(self->field_0028));
            self->field_0028 = 0xFFFFFFFFu;
        }
        result = CCharActionFsm_ClearActionFlag(self, 0x20u);
        self->field_00bc = 0;
        break;
    default:
        result = sub_480110(self, nullptr, contextArg);
        break;
    }

    return static_cast<char>(result);
}

HOOK(0x488160, CCharActionFsm_ExitCurrentState, "auto");
