#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Bound by address: TCharTask base-class prepare routine (__thiscall, no stack args).
CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));

// Bound by address: AI subsystem reset, called with (1, -1) (__fastcall callee, 2 stack args).
CSF_FUNC(int, __fastcall, AiSubsystem_ResetWithFlags, 0x00492C10, (void* self, void* edx, int a1, int a2));

// Clears the pending action of a TCharTask and resets the owner's state.
static int __fastcall TCharTask_ResetPendingAction(TCharTask* self, void* /*edx*/)
{
    CharTaskBase_Prepare(self, nullptr);

    // owner = self->field_0008
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));
    // inner = owner->[0x14]
    uint8_t* inner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x14));
    // v2 = inner->[0x30]
    uint8_t* actor = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, inner, 0x30));

    CSF_AT(int, actor, 0x1c) = 0;
    ((void (__fastcall*)(void*, void*))CSF_AT(void**, actor, 0)[160 / 4])(actor, nullptr);

    CSF_AT(uint32_t, owner, 0x18) &= ~1u;

    int result = AiSubsystem_ResetWithFlags(CSF_AT(uint32_t, owner, 0x14) == 0 ? nullptr : reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14)), nullptr, 1, -1);

    CSF_AT(uint8_t, actor, 0x230) = 0;
    return result;
}

HOOK(0x004D7EC0, TCharTask_ResetPendingAction, "auto");
