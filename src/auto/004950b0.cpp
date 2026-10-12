#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CCharActionFsm_GetSlot28ItemIfUsable, 0x00487260, (CCharActionFsm* self, void*));

// Returns the scene-selection handle stored at +0x398 unless the character's
// action FSM holds a slot-0x28 item that reports itself as not usable, in
// which case there is no alternate selection (returns 0).
static int __fastcall CPlayerCharacter_GetAlternateSceneSelection(CPlayerCharacter* self, void* /*edx*/)
{
    uint32_t selection = self->field_0398;
    if (selection != 0)
    {
        CCharActionFsm* actionFsm = reinterpret_cast<CCharActionFsm*>(self->field_0044);
        int slotItem = CCharActionFsm_GetSlot28ItemIfUsable(actionFsm, nullptr);
        if (slotItem != 0)
        {
            void* itemObj = reinterpret_cast<void*>(static_cast<uintptr_t>(slotItem));
            // Virtual slot +0x2C (index 11) on the item's vtable; no arguments.
            uint8_t isUsable = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, itemObj, 0)[11])(itemObj, nullptr);
            if (!isUsable)
                return 0;
        }
    }
    return static_cast<int>(selection);
}

HOOK(0x004950B0, CPlayerCharacter_GetAlternateSceneSelection, "auto");
