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

CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructTaskBase, 0x004D1660, (void* self, void* edx, int a2));

// Constructor for C_0083b4a0 (autonomous AI task). Base construction first, then the
// vtable chain and default field values. Original is __thiscall with ret 4.
static uint32_t* __fastcall C_0083b4a0_ConstructAutonomousTask(C_0083b4a0* self, void* /*edx*/, int a2) {
    AiAutonomous_ConstructTaskBase(self, nullptr, a2);

    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x0083B4A0);
    CSF_AT(void*, self, 0x04) = CSF_VTBL(0x0083B484);
    self->field_004c = 0xFFFFFFFFu;
    self->field_0050 = 0xFFFFFFFFu;
    CSF_AT(void*, self, 0x48) = CSF_VTBL(0x0082D448);
    self->field_004c = 0xFFFFFFFFu;
    self->field_0050 = 0xFFFFFFFFu;
    self->field_0044 = 0;
    self->field_0040 = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4CFA20, C_0083b4a0_ConstructAutonomousTask, "auto");
