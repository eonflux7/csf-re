#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00833ee8_PrepareDispatch, 0x0049BAF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CCharActionFsm_GetSlot28ItemIfUsable, 0x00487260, (CCharActionFsm* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_SetCharacterItem, 0x00494E90, (void* self, void* edx, int a4));

static int __fastcall C_00833ee8_DispatchSceneObjectAction(C_00833ee8* self, void* /*edx*/, int a2)
{
    C_00833ee8_PrepareDispatch(self, nullptr, a2);

    CCharActionFsm* fsm = reinterpret_cast<CCharActionFsm*>(self->field_0044);
    int result = CCharActionFsm_GetSlot28ItemIfUsable(fsm, nullptr);
    if (result) {
        int slot28Item = CCharActionFsm_GetSlot28ItemIfUsable(fsm, nullptr);
        int itemHandle = CSF_AT(int, reinterpret_cast<void*>(slot28Item), 0x80);
        result = CharActionFsm_SetCharacterItem(self, nullptr, itemHandle);
        CSF_AT(int, reinterpret_cast<void*>(self->field_0030), 0x20) = 0;
    }
    return result;
}

HOOK(0x0049BF00, C_00833ee8_DispatchSceneObjectAction, "auto");
