#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_00833908_SetSceneCallback, 0x0049F560, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_ConfigureMissionInterface, 0x0049F5A0, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, TCharTask_BaseInit_4D0350, 0x004D0350, (void* self, void* edx, int a2));

static C_0083dd58* __fastcall C_0083dd58_Construct(C_0083dd58* self, void* /*edx*/, int a2)
{
    TCharTask_BaseInit_4D0350(self, nullptr, a2);

    self->vtbl = CSF_VTBL(0x0083DD58);
    self->field_0004 = (uint32_t)CSF_ADDR_VT(0x0083DD3C);
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_008c = 0;

    void* owner = reinterpret_cast<void*>(self->field_0014);

    self->field_0070 = 0;
    self->field_0074 = 1;
    self->field_0090 = 0;
    self->field_0091 = 1;
    self->field_0092 = 0;

    C_00833908_SetSceneCallback(owner, nullptr, reinterpret_cast<void*>(1));
    C_00833908_ConfigureMissionInterface(owner, nullptr, 0, 1051, 1000, 1);

    CSF_AT(uint32_t, owner, 0x1A8) = 0;
    CSF_AT(uint32_t, owner, 0x1AC) = 0;

    return self;
}

HOOK(0x004DA950, C_0083dd58_Construct, "auto");
