#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_485AC0, 0x00485AC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

// Original: void __thiscall (CCharActionFsm*), ret 0.
// If the held item object (field_00a4) is set and has no remaining items, either
// request a reload through the weapon FSM (when sub_485AC0 says so) or clear field_0090.
static void __fastcall CCharActionFsm_ServiceEmptyItemAndRequestReload(CCharActionFsm* self, void* /*edx*/)
{
    if (self->field_00a4 == 0)
        return;

    void* itemObject = reinterpret_cast<void*>(self->field_00a4);

    // Virtual slot 0xE0 on the item object (vtbl index 56), __thiscall with no args.
    ((void(__fastcall*)(void*, void*))(CSF_AT(void**, itemObject, 0)[0xE0 / 4]))(itemObject, nullptr);

    if (GetCurrentItemCount(reinterpret_cast<void*>(self->field_00a4), nullptr) != 0)
        return;

    if (sub_485AC0(self, nullptr) != 0)
    {
        CCharWeaponFsm* weaponFsm = reinterpret_cast<CCharWeaponFsm*>(self->field_0008);
        uint32_t* requestTarget = &weaponFsm->field_0004;

        int32_t requestArgs[2];
        requestArgs[0] = 1;
        requestArgs[1] = -1;

        // Virtual slot 0x6C on the object stored in field_0004 (vtbl index 27), __thiscall(this=&field_0004, args).
        void* requestObject = reinterpret_cast<void*>(*requestTarget);
        ((void(__fastcall*)(void*, void*, int32_t*))(CSF_AT(void**, requestObject, 0)[0x6C / 4]))(
            requestTarget, nullptr, requestArgs);
    }
    else
    {
        self->field_0090 = 0xFFFFFFFFu;
    }
}

HOOK(0x00486460, CCharActionFsm_ServiceEmptyItemAndRequestReload, "auto");
