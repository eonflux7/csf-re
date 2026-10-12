#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharActionFsm_ForwardToStateDispatch, 0x00482840, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CCharActionFsm_UpdateItemActions, 0x00488210, (CCharActionFsm* self, void* , int updateArgument));

// Enables/disables the player character's action handling. A request to enable is
// turned into a disable (virtual slot 0x6C with 0) while the character is in a
// state that forbids it; otherwise the request is forwarded to the owned
// controller object and to the action FSM.
static void __fastcall CPlayerCharacter_SetActionsEnabled(CPlayerCharacter* self, void* /*edx*/, int enable)
{
    const bool enableFlag = static_cast<uint8_t>(enable) != 0;

    if (enableFlag && self->field_0228 == 0)
    {
        bool blocked = false;

        if (self->field_0398 != 0)
        {
            blocked = true;
        }
        else
        {
            const uint32_t stateFlags = self->field_02e0;
            if ((stateFlags & 0x2000) != 0)
                blocked = true;
            else if ((stateFlags & 0x10000) != 0 && self->field_03d0 == 0)
                blocked = true;
            else if (self->field_03de != 0)
                blocked = true;
        }

        if (blocked)
        {
            self->vSlot6C(0);
            return;
        }
    }

    void* controller = reinterpret_cast<void*>(self->vGetField016c());
    ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, controller, 0)[0x3C / 4])(controller, nullptr, enable);

    if (self->field_0044 != 0)
    {
        if (!enableFlag || ((self->field_02e0 & 0x2100) == 0 && self->field_0398 == 0))
        {
            CharActionFsm_ForwardToStateDispatch(reinterpret_cast<void*>(self->field_0044), nullptr, enable);
        }
        CCharActionFsm_UpdateItemActions(reinterpret_cast<CCharActionFsm*>(self->field_0044), nullptr, enable);
    }
}

HOOK(0x00495350, CPlayerCharacter_SetActionsEnabled, "auto");
