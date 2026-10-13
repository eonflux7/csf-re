#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_00833908_SetSceneCallback, 0x0049F560, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_ConfigureMissionInterface, 0x0049F5A0, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, C_0083b668_InitBaseTask, 0x004D0350, (C_0083b668* self, void* , int a2));

static void* __fastcall C_00835228_Construct(C_00835228* self, void* /*edx*/, int taskArgument)
{
    C_0083b668_InitBaseTask(reinterpret_cast<C_0083b668*>(self), nullptr, taskArgument);

    self->vtbl = CSF_VTBL(0x00835228);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00835208));
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_0094 = 0;
    self->field_00a0 = 1;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_00b8 = 0;
    self->field_00bc = 0;
    self->field_00c0 = 0;
    self->field_00c4 = 0;
    self->field_009c = reinterpret_cast<uint32_t>(CSF_VTBL(0x00832F18));
    self->field_00d0 = 0;
    self->field_00d4 = 0;
    self->field_00d8 = 0;
    self->field_00c8 = 0;

    void* scene = reinterpret_cast<void*>(self->field_0014);
    self->field_0070 = 0;
    self->field_0078 = 0.0f;
    self->field_0098 = 1;
    self->field_0074 = 1;
    self->field_0099 = 0;
    self->field_009a = 1;

    C_00833908_SetSceneCallback(scene, nullptr, reinterpret_cast<void*>(1));
    C_00833908_ConfigureMissionInterface(scene, nullptr, 0, 1051, 1000, 5);
    CSF_AT(uint32_t, scene, 0x1A8) = 0;
    CSF_AT(uint32_t, scene, 0x1AC) = 0;

    return self;
}

HOOK(0x004A9A80, C_00835228_Construct, "auto");
