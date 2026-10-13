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

// C_00860178::Construct (base-class constructor), called first by the original.
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* edx));

// Resets a CRenderEngine to its default configuration: 800x600 window, 640x480
// secondary mode, 32 bpp, and clears the remaining flags and the trailing block.
static CRenderEngine* __fastcall CRenderEngine_InitDefaults(CRenderEngine* self, void* /*edx*/)
{
    C_00860178_Construct(self, nullptr);
    self->vtbl = CSF_VTBL(0x00861328);

    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_002a = 0;
    self->field_002b = 0;
    self->field_002c = 0;

    self->field_0004 = 800;
    self->field_0008 = 600;
    self->field_000c = 640;
    self->field_0010 = 480;
    self->field_0014 = 32;

    self->field_0018 = 1;
    self->field_0024 = 1;
    self->field_0028 = 1;
    self->field_0029 = 1;

    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0034 = 0;
    self->field_0058 = 0;
    self->field_0030 = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;

    self->field_0040 = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;

    return self;
}

HOOK(0x006DBCD0, CRenderEngine_InitDefaults, "auto");
