#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x004E6000, __thiscall, ret 0 (one stack arg, popped by the callee)
CSF_FUNC(void, __fastcall, RunStepOnField000C, 0x004E6000, (void* self, void* edx, uint32_t* arg0));

// Original: 0x004E6410. Calls the virtual step (vtbl slot 0xA4), runs the helper on
// the sub-field at +0x0C, then returns the dword at +0x14.
static uint32_t __fastcall C_0083e9d0_ComputeValue(C_0083e9d0* self, void* /*edx*/)
{
    self->vSlotA4();
    RunStepOnField000C(self, nullptr, &self->field_000c);
    return self->field_0014;
}

HOOK(0x004E6410, C_0083e9d0_ComputeValue, "auto");
