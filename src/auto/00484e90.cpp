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
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, CCharActionFsm_AdvanceTimedState, 0x00483D90, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, CCharActionFsm_ClearItemTarget, 0x004847C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_UnlinkItemHandle, 0x004829B0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, SubStateTickA, 0x0048D9C0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, SubStateTickB, 0x0048DDA0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, SubStateTickC, 0x0048DEE0, (void* self, void* edx, float a2));
CSF_FUNC(bool, __fastcall, IsWeaponSlotBusy, 0x004950F0, (void* self, void* edx));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Dispatches on the current phase id (field_0010). Phase ids 0x0A..0x29 map to
// the per-phase handlers; anything else returns phase - 10 unchanged.
static int __fastcall CCharActionFsm_DispatchPhaseAction(CCharActionFsm* self, void* /*edx*/, float elapsedSeconds)
{
    const uint32_t phase = self->field_0010;
    int result = static_cast<int>(phase - 10);

    switch (phase)
    {
    case 0xAu:
        result = CCharActionFsm_AdvanceTimedState(self, nullptr, elapsedSeconds);
        break;

    case 0xBu:
        result = self->field_0069;
        if (result)
        {
            self->vUpdateWeaponSelection(0);
            result = self->vBeginWeaponStateChange(0, 0);
        }
        break;

    case 0x21u:
        result = self->field_0069;
        if (result)
            result = self->vSetStateAndDispatch(0x22);
        break;

    case 0x22u:
        result = self->field_0069;
        if (result)
        {
            if (self->field_00a4)
            {
                C_00833908* weaponFsm = reinterpret_cast<C_00833908*>(self->field_0008);
                int slotIndex = CSF_AT(int, weaponFsm, 0x2bc);
                if (slotIndex >= 0)
                {
                    int handleAddress = static_cast<int>(
                        reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(weaponFsm) + 0x2b4));
                    int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleAddress);
                    if (resolved)
                    {
                        void* item = reinterpret_cast<void*>(self->field_00a4);
                        // vtbl slot 0x44 / 4 = 17, one argument (resolved + 28)
                        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, item, 0)[0x44 / 4])(
                            item, nullptr, resolved + 28);
                    }
                }
                CCharActionFsm_ClearItemTarget(self, nullptr);
                self->field_00a4 = 0;
                CCharActionFsm_UnlinkItemHandle(self, nullptr);
            }

            if (self->vGetIndexedEntryOrZero())
            {
                int chainedValue = self->vGetIndexedEntryOrZero();
                void* chained = reinterpret_cast<void*>(static_cast<uintptr_t>(chainedValue));
                // vtbl slot 0x30 / 4 = 12, argument 1
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, chained, 0)[0x30 / 4])(
                    chained, nullptr, 1);
            }

            if (!IsWeaponSlotBusy(reinterpret_cast<void*>(self->field_0008), nullptr))
            {
                void* weaponObj = reinterpret_cast<void*>(self->field_0008);
                // vtbl slot 0x6C / 4 = 27, argument 0
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, weaponObj, 0)[0x6c / 4])(
                    weaponObj, nullptr, 0);
            }

            result = self->vSetStateAndDispatch(0x23);
        }
        break;

    case 0x23u:
        result = self->field_0069;
        if (result)
            result = self->vBeginWeaponStateChange(0, 0);
        break;

    case 0x24u:
    {
        C_00833908* weaponFsm = reinterpret_cast<C_00833908*>(self->field_0008);
        int slotIndex = CSF_AT(int, weaponFsm, 0x2bc);
        bool useEntry = false;
        int entryData = 0;
        if (slotIndex >= 0)
        {
            int handleAddress = static_cast<int>(
                reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(weaponFsm) + 0x2b4));
            int entry = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleAddress);
            if (entry != 0)
            {
                void* entryObj = reinterpret_cast<void*>(static_cast<uintptr_t>(entry));
                if (CSF_AT(int, entryObj, 0x50) == 0x1a)
                {
                    entryData = CSF_AT(int, entryObj, 0x34);
                    void* dataObj = reinterpret_cast<void*>(static_cast<uintptr_t>(entryData));
                    useEntry = (CSF_AT(uint8_t, dataObj, 0x44) & 0xC2) == 0;
                }
            }
        }
        if (useEntry)
            result = entryData;
        else
            result = self->vBeginWeaponStateChange(0, 0);
        break;
    }

    case 0x25u:
    case 0x26u:
        result = self->field_00bc;
        if (!result)
            result = self->vBeginWeaponStateChange(0, 0);
        break;

    case 0x27u:
        // The original loads this = [self+0xD4] before the tail jump.
        return static_cast<int>(reinterpret_cast<uintptr_t>(
            SubStateTickA(reinterpret_cast<void*>(self->field_00d4), nullptr, elapsedSeconds)));

    case 0x28u:
        return SubStateTickB(reinterpret_cast<void*>(self->field_00d4), nullptr, elapsedSeconds);

    case 0x29u:
        return SubStateTickC(reinterpret_cast<void*>(self->field_00d4), nullptr, elapsedSeconds);

    default:
        break;
    }

    return result;
}

HOOK(0x484E90, CCharActionFsm_DispatchPhaseAction, "auto");
