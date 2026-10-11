#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d58c_RefreshState, 0x004054F0, (void* self, void* edx));

static bool __fastcall C_0082d58c_Initialize(C_0082d58c* self, void* /*edx*/, int a2) {
    self->field_0008 = a2;
    self->field_000c = 10.0f;
    self->field_0010 = 0;
    self->field_0011 = 0;
    self->field_0018 = 0.0f;
    self->field_0014 = 0;
    self->field_0020 = 0.0f;
    self->field_001c = 0.0f;
    C_0082d58c_RefreshState(self, nullptr);
    return true;
}

HOOK(0x004059D0, C_0082d58c_Initialize, "auto");
