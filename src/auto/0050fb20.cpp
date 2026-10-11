#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(void, __fastcall, ProcessUnflaggedTailObjects, 0x0071AB50, (void* self, void*));
CSF_FUNC(void, __fastcall, MenuMessageBox_UpdateChildren, 0x005287F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, sub_50A080, 0x0050A080, ());
CSF_FUNC(int, __fastcall, sub_50F2F0, 0x0050F2F0, (void* self, void* edx));

// Refreshes the menu frame, then tail-jumps into the menu object's slot 0x78.
static hook::Tail __fastcall BddLoader_RefreshMenuFrame(void* self, const uint32_t* args) {
    (void)args;
    (void)CSF_VTBL(0x0050F2F0);

    S_0050f2f0* owner = reinterpret_cast<S_0050f2f0*>(self);

    void* menuObj = MenuControls_ResolveObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    int result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, menuObj, 0)[0x74 / 4])(menuObj, nullptr, owner->field_0004 == 1 ? 1 : 0);

    if ((result & 0xFF) == 0) {
        return hook::tail_ret(result);
    }

    ProcessUnflaggedTailObjects(reinterpret_cast<void*>(owner->field_0014), nullptr);

    if (owner->field_0080) {
        MenuMessageBox_UpdateChildren(reinterpret_cast<void*>(owner->field_0080), nullptr);
    }

    uint8_t* globalObj = reinterpret_cast<uint8_t*>(GetGlobalInt());
    if (CSF_AT(uint32_t, globalObj, 0x8C) != 0) {
        void* frameObj68 = reinterpret_cast<void*>(owner->field_0068);
        if (CSF_AT(uint32_t, frameObj68, 0x24) != 0) {
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, frameObj68, 0)[0x10 / 4])(frameObj68, nullptr);
        }
    }

    void* taskObj74 = reinterpret_cast<void*>(owner->field_0074);
    if (taskObj74 != nullptr) {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, taskObj74, 0)[0x18 / 4])(taskObj74, nullptr);
    }

    sub_50A080();
    sub_50F2F0(self, nullptr);

    void* finalMenuObj = MenuControls_ResolveObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    return hook::tail_jump(finalMenuObj, (*reinterpret_cast<void***>(finalMenuObj))[0x78 / 4]);
}

HOOK_TAIL(0x0050FB20, BddLoader_RefreshMenuFrame, "game_bdd_loader");
