#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x004A1CD0: no stack args, returns int, called with ecx = this.
CSF_FUNC(int, __fastcall, sub_4A1CD0, 0x004A1CD0, (void* self, void* edx));

// Resets the attachment state of a HUD alert icon object, stores the owner
// argument, then forwards to the base initializer at 0x4A1CD0 and returns its result.
static int __fastcall HudAlertIcons_InitAttachment(S_004a1cd0* self, void* /*edx*/, int ownerArg) {
    self->field_0010 = 0;
    self->field_000c = 0;
    self->field_0004 = 0;
    self->field_0000 = 0;
    self->field_0011 = 0;
    self->field_0014 = 0xFFFFFFFFu;
    self->field_0018 = 0xFFFFFFFFu;
    self->field_001c = 0xFFFFFFFFu;
    self->field_0020 = 0xBF800000u;  // -1.0f bit pattern, as stored by the original
    self->field_0008 = static_cast<uint32_t>(ownerArg);
    return sub_4A1CD0(self, nullptr);
}

HOOK(0x004A1D30, HudAlertIcons_InitAttachment, "auto");
