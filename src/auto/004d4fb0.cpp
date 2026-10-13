#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (TFncGirar* self, void* , int a2));
CSF_FUNC(bool, __fastcall, CharTask_IsOwnerLookReady, 0x004D4C80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Virtual slot signatures on the target object (vtable slots 46 and 47).
using TargetIsBusyFn = bool(__fastcall*)(void* self, void* edx);
using TargetGetFlagsFn = uint32_t*(__fastcall*)(void* self, void* edx, uint32_t* out);

// Checks the target's state and either finishes the rotation or proceeds with the look-at step.
static void __fastcall TCharTask_UpdateRotationTurn(TCharTask* self, void* /*edx*/, int a2)
{
    (void)a2;

    // ownerObj = self->field_0008; target = ownerObj->field_0014->field_0030
    uint8_t* ownerObj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    uint8_t* ownerSub = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, ownerObj, 0x14));
    void* targetObj = reinterpret_cast<void*>(CSF_AT(uint32_t, ownerSub, 0x30));

    void** targetVtbl = CSF_AT(void**, targetObj, 0);

    bool targetIsBusy = ((TargetIsBusyFn)targetVtbl[46])(targetObj, nullptr);
    if (targetIsBusy) {
        TFncGirar_OnRotationComplete(reinterpret_cast<TFncGirar*>(self), nullptr, 1);
        return;
    }

    uint32_t flagsOut = 0;
    uint32_t* flagsPtr = ((TargetGetFlagsFn)targetVtbl[47])(targetObj, nullptr, &flagsOut);
    if ((*flagsPtr & 2) == 0) {
        TFncGirar_OnRotationComplete(reinterpret_cast<TFncGirar*>(self), nullptr, 1);
        return;
    }

    if (CharTask_IsOwnerLookReady(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08)), nullptr)) {
        int lookOffset = static_cast<int>(CSF_AT(uint32_t, self, 0x0C)) + 60;
        CharTask_LookupByOffset(reinterpret_cast<void*>(self), nullptr, lookOffset);
    }
}

HOOK(0x004D4FB0, TCharTask_UpdateRotationTurn, "auto");
