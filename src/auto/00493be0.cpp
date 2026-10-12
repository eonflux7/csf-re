#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_FUNC(int, __fastcall, CallVirtualAt0x134With2And0, 0x00483530, (void* self, void*));

static void __fastcall CPlayerCharacter_UpdateTimedBehavior(CPlayerCharacter* self, void* /*edx*/, float elapsedSeconds)
{
    auto* stateOwner = reinterpret_cast<uint8_t*>(self->field_004c);
    uint8_t desiredState = CSF_AT(uint8_t, stateOwner, 0x89);

    if (self->field_03de == desiredState)
    {
        self->field_03e0 = 0.5f;
        return;
    }

    self->field_03e0 = self->field_03e0 - elapsedSeconds;

    if (desiredState != 0 || self->field_03e0 < g_fZero)
    {
        uint8_t wasLocked = self->field_0228;
        self->field_03de = desiredState;
        if (!wasLocked)
        {
            self->vSlot6C(desiredState == 0 ? 1 : 0);
        }
        if (self->field_03de == 0)
        {
            CallVirtualAt0x134With2And0(reinterpret_cast<void*>(self->field_0044), nullptr);
        }
    }
}

HOOK(0x00493BE0, CPlayerCharacter_UpdateTimedBehavior, "auto");
