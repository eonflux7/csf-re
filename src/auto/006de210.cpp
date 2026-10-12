#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_006dcf90_RefreshSettings, 0x006DD040, (void* self, void* edx));

static int* __fastcall S_006dcf90_InitializeSettings(S_006dcf90* self, void* /*edx*/)
{
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 1065353216;
    self->field_0060 = 1065353216;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00b8 = 0;
    self->field_00d8 = 0;
    self->field_00dc = 0;
    self->field_00e0 = 0;
    self->field_00f4 = 0;
    self->field_00f8 = 0;
    self->field_00fc = 0;
    self->field_00d4 = 0;
    self->field_00e4 = 0;
    self->field_00e8 = 0;
    self->field_00ec = 0;
    self->field_00f0 = 1086918619;
    self->field_010c = 0;
    self->field_0110 = 0;
    self->field_0114 = 0;
    self->field_0118 = 0;
    self->field_011c = 0;
    self->field_0120 = 0;
    self->field_0124 = 0;
    self->field_0128 = 0;
    self->field_012c = 0;
    self->field_0130 = 0;
    self->field_0134 = 0;
    self->field_0138 = 0;
    self->field_013c = 0;
    self->field_0140 = 0;
    self->field_0144 = 0;
    self->field_0148 = 0;
    self->field_014c = 0;
    self->field_0150 = 0;
    self->field_0004 = 0;

    S_006dcf90_RefreshSettings(self, nullptr);
    return reinterpret_cast<int*>(self);
}

HOOK(0x006DE210, S_006dcf90_InitializeSettings, "auto");
