#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, AppConfig_InitializeValues, 0x00760030, (void* a1, int a2));

static S_0055af00* __fastcall S_0055af00_Initialize(S_0055af00* self, int argument)
{
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0028 = 0;
    self->field_002c = 15;
    reinterpret_cast<uint8_t*>(&self->field_0018)[0] = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0030 = -1;
    self->field_0068 = 15;
    self->field_0064 = 0;
    reinterpret_cast<uint8_t*>(&self->field_0054)[0] = 0;
    self->field_0084 = 15;
    self->field_0080 = 0;
    reinterpret_cast<uint8_t*>(&self->field_0070)[0] = 0;
    self->field_00a4 = 15;
    self->field_00a0 = 0;
    reinterpret_cast<uint8_t*>(&self->field_0090)[0] = 0;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00e4 = 15;
    self->field_00e0 = 0;
    reinterpret_cast<uint8_t*>(&self->field_00d0)[0] = 0;
    self->field_0104 = 15;
    self->field_0100 = 0;
    reinterpret_cast<uint8_t*>(&self->field_00f0)[0] = 0;

    AppConfig_InitializeValues(self, argument);
    return self;
}

HOOK(0x00760E20, S_0055af00_Initialize, "auto");
