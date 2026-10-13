#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards the update to the child task, then, if the flag at +0x14 is set,
// notifies the rotation component that the rotation has completed.
CSF_FUNC(void, __fastcall, TCharTask_ForwardUpdateToChild, 0x004DB0F0, (TCharTask* self, void* /*edx*/, int a2));
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (TFncGirar* self, void* /*edx*/, int a2));

static void __fastcall TCharTask_UpdateAndFinishRotation(TCharTask* self, void* /*edx*/, int a2)
{
    TCharTask_ForwardUpdateToChild(self, nullptr, a2);
    if (CSF_AT(uint8_t, self, 0x14) != 0)
    {
        TFncGirar_OnRotationComplete(reinterpret_cast<TFncGirar*>(self), nullptr, 1);
    }
}

HOOK(0x004A73E0, TCharTask_UpdateAndFinishRotation, "auto");
