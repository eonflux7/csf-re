#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x004D10F0, __fastcall (ecx = this), returns int, pops 0.
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void* edx));

static int __fastcall C_0083aa80_ResetCombatState(C_0083aa80* self, void* /*edx*/)
{
    int result;

    self->field_0604 = 0xFFFFFFFFu;
    self->field_0608 = 0xFFFFFFFFu;
    self->field_060c = 0xFFFFFFFFu;
    self->field_0614 = 0xFFFFFFFFu;
    self->field_0618 = 0xFFFFFFFFu;
    self->field_062c = 0xFFFFFFFFu;
    self->field_0630 = 0xFFFFFFFFu;
    self->field_0638 = 0;
    self->field_063c = 1065353216u;   // 1.0f bit pattern
    self->field_0678 = 2139095039u;   // FLT_MAX bit pattern
    self->field_0670 = 0;
    self->field_0684 = 0;
    self->field_0685 = 0;
    self->field_067c = 0;
    self->field_0680 = 0;

    // Sub-object embedded at +0x690 (ecx = this + 0x690).
    void* subObject = static_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x690);
    result = C_004D10F0_ResetFields(subObject, nullptr);

    self->field_06c8 = 0;
    return result;
}

HOOK(0x4B9720, C_0083aa80_ResetCombatState, "auto");
