#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_ConstructObjectController, 0x004A4280, (void* self, void* edx, int a2));

static int __fastcall C_00834598_InitializeForSceneObject(C_00834598* self, void* /*edx*/, int a2)
{
    SceneMissionSetup_ConstructObjectController(self, nullptr, a2);

    *(void***)self = CSF_VTBL(0x00834598);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x00834578);
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 10;
    self->field_00a4 = 0;
    self->field_00a8 = 15;
    CSF_AT(uint8_t, self, 0x94) = 0;
    self->field_008c = 0;
    self->field_0088 = 1;

    return (int)self;
}

HOOK(0x4A5760, C_00834598_InitializeForSceneObject, "auto");
