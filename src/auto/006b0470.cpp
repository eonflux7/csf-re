#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_6B03B0, 0x006B03B0, (void* self, void* edx, int a2));

static S_006b0320* __fastcall S_006b0320_Init(S_006b0320* self, void* /*edx*/, int a2) {
    self->field_0000 = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 10;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 10;
    sub_6B03B0(self, nullptr, a2);
    return reinterpret_cast<S_006b0320*>(&self->field_0000);
}

HOOK(0x006B0470, S_006b0320_Init, "auto");
