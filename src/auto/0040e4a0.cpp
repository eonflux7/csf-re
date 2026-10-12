#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t*, __fastcall, Base_0082de40_Construct, 0x0040F720, (void* self, void* edx));

static uint32_t* __fastcall C_0082dd78_Initialize(C_0082dd78* self, void* /*edx*/, char a2) {
    Base_0082de40_Construct(self, nullptr);
    *(void***)self = CSF_VTBL(0x0082DD78);
    self->field_0034 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0038 = static_cast<uint8_t>(a2);
    self->field_0030 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0.0f;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0074 = 0;
    self->field_0039 = 1;
    self->field_0070 |= 0x20u;
    self->field_0034 = 0;
    self->field_0034 = 1;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0040E4A0, C_0082dd78_Initialize, "auto");
