#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* /*edx*/));

static C_00861b58* __fastcall C_00861b58_ConstructShapeWrapper(C_00861b58* self, void* /*edx*/, int a2) {
    C_00860178_Construct(self, nullptr);
    self->field_0004 = static_cast<uint32_t>(a2);
    self->vtbl = CSF_VTBL(0x00861B58);
    return self;
}

HOOK(0x006E4200, C_00861b58_ConstructShapeWrapper, "auto");
