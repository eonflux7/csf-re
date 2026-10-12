#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, RpPrtStd_ResetEmitterSubState0920, 0x006E0920, (void* self, void* edx));
CSF_FUNC(int, __fastcall, RpPrtStd_ResetEmitterSubState0E00, 0x006E0E00, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, RpPrtStd_ResetEmitterSubObject703440, 0x00703440, (void* self, void* edx));
CSF_FUNC(int, __fastcall, RpPrtStd_ResetEmitterSubObject703670, 0x00703670, (void* self, void* edx));

static unsigned int* __fastcall RpPrtStd_InitEmitterState(S_0041e5f0* self, void* /*edx*/)
{
    self->field_0000 = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 10;

    void* subObject = reinterpret_cast<void*>(&self->field_0020);
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 10;
    RpPrtStd_ResetEmitterSubObject703440(subObject, nullptr);

    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 10;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 10;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 0;

    RpPrtStd_ResetEmitterSubState0920(self, nullptr);
    RpPrtStd_ResetEmitterSubState0E00(self, nullptr);
    RpPrtStd_ResetEmitterSubObject703670(subObject, nullptr);

    return &self->field_0000;
}

HOOK(0x6E26E0, RpPrtStd_InitEmitterState, "auto");
