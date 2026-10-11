#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_00833908_SetSceneCallback, 0x0049F560, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_ConfigureMissionInterface, 0x0049F5A0, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, TCharTask_BaseInit_4D0350, 0x004D0350, (void* self, void* edx, int a2));

static void* __fastcall C_0083ac40_Initialize(
    C_0083ac40* self, void* /*edx*/, int initializationArgument)
{
    TCharTask_BaseInit_4D0350(self, nullptr, initializationArgument);

    self->vtbl = CSF_VTBL(0x0083AC40);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083AC20));
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_00a8 = 1;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00b8 = 0;
    self->field_00c0 = 0;
    self->field_00c4 = 0;
    self->field_00c8 = 0;
    self->field_00cc = 0;
    self->field_00a4 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00832F18));
    self->field_00d8 = 0;
    self->field_00dc = 0;
    self->field_00e0 = 0;
    self->field_00d0 = 0;

    void* scene = reinterpret_cast<void*>(self->field_0014);
    self->field_0070 = 0;
    self->field_0084 = 0.0f;
    self->field_00e4 = 1;
    self->field_0074 = 1;
    self->field_00e5 = 0;
    self->field_00e6 = 1;

    C_00833908_SetSceneCallback(scene, nullptr, reinterpret_cast<void*>(6));
    C_00833908_ConfigureMissionInterface(scene, nullptr, 0, 1051, 1005, 1);
    C_00833908_ConfigureMissionInterface(scene, nullptr, 1, 1072, 1004, 1);

    int interfaceId = 1052;
    int remaining = 4;
    do
    {
        C_00833908_ConfigureMissionInterface(
            scene, nullptr, 2, interfaceId, interfaceId - 52, 0);
        ++interfaceId;
        --remaining;
    } while (remaining != 0);

    CSF_AT(uint32_t, scene, 0x1AC) = 0;
    CSF_AT(uint32_t, scene, 0x1A8) = 0;
    return self;
}

HOOK(0x004CE6F0, C_0083ac40_Initialize, "auto");
