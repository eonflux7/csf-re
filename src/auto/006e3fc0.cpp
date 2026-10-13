#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* /*edx*/));

static C_00861b40* __fastcall C_00861b40_ConstructShapeWrapper(C_00861b40* self, void* /*edx*/, int a2) {
    C_00860178_Construct(self, nullptr);
    CSF_AT(int, self, 0x4) = a2;
    self->vtbl = CSF_VTBL(0x00861B40);
    return self;
}

HOOK(0x006E3FC0, C_00861b40_ConstructShapeWrapper, "auto");
