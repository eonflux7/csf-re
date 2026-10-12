#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0048f360_ResetState_48F360, 0x0048F360, (S_0048f360* self, void*));

static int __fastcall S_0048f360_InvalidateFields_49DEC0(S_0048f360* self, void* /*edx*/) {
    (void)CSF_VTBL(0x0048F360);

    S_0048f360_ResetState_48F360(self, nullptr);

    self->field_0180 = static_cast<uint32_t>(-1);
    self->field_0184 = static_cast<uint32_t>(-1);
    self->field_0188 = static_cast<uint32_t>(-1);
    self->field_018c = 0;
    self->field_0178 = 0;
    self->field_018d = 0;
    self->field_01a0 = static_cast<uint32_t>(-1);
    self->field_01a4 = static_cast<uint32_t>(-1);
    return -1;
}

HOOK(0x0049DEC0, S_0048f360_InvalidateFields_49DEC0, "auto");
