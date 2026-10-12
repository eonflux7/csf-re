#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructor-style initializer for C_00862874 (scene controller).
// Original: 0x006F62D0, __thiscall, ret 0. Returns `this`.
static C_00862874* __fastcall GfxScene_ConstructSceneController(C_00862874* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x00862874);
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 10;
    self->field_0018 = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_0058 = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    return self;
}

HOOK(0x006F62D0, GfxScene_ConstructSceneController, "auto");
