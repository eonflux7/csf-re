#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, TCharTask_BaseInit_4D0350, 0x004D0350, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, C_00833908_SetSceneCallback, 0x0049F560, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_ConfigureMissionInterface, 0x0049F5A0, (void* self, void* edx, int a2, int a3, int a4, int a5));

static C_0083b850* __fastcall C_0083b850_InitMissionTask(C_0083b850* self, void* /*edx*/, int a2)
{
    TCharTask_BaseInit_4D0350(self, nullptr, a2);

    self->vtbl = CSF_VTBL(0x0083B850);
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_VT(0x0083B834)));
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 0;

    C_00833908* missionInterface = reinterpret_cast<C_00833908*>(self->field_0014);
    self->field_0070 = 0;

    C_00833908_SetSceneCallback(missionInterface, nullptr,
                                reinterpret_cast<void*>(static_cast<uintptr_t>(2)));
    C_00833908_ConfigureMissionInterface(missionInterface, nullptr, 0, 1000, 1000, 1);
    C_00833908_ConfigureMissionInterface(missionInterface, nullptr, 1, 1001, 1001, 0);

    missionInterface[1].field_0024 = 0;
    missionInterface[1].field_0028 = 0;

    return self;
}

HOOK(0x004D0E60, C_0083b850_InitMissionTask, "auto");
