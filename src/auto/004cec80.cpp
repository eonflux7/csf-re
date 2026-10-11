#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, TCharTask_BaseInit_4D0350, 0x004D0350, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, C_00833908_SetSceneCallback, 0x0049F560, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_ConfigureMissionInterface, 0x0049F5A0, (void* self, void* edx, int a2, int a3, int a4, int a5));

static unsigned int** __fastcall C_0083ae28_InitMissionInterface(C_0083ae28* self, void* /*edx*/, int a2)
{
    TCharTask_BaseInit_4D0350(self, nullptr, a2);

    C_00833908* missionInterface = reinterpret_cast<C_00833908*>(self->field_0014);
    self->vtbl = CSF_VTBL(0x0083AE28);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083AE0C));

    C_00833908_SetSceneCallback(missionInterface, nullptr, reinterpret_cast<void*>(9));
    C_00833908_ConfigureMissionInterface(missionInterface, nullptr, 0, 1000, 1000, 1);

    int meshIndex = 1051;
    int remaining = 8;
    do
    {
        C_00833908_ConfigureMissionInterface(missionInterface, nullptr, 1, meshIndex, meshIndex - 49, 0);
        ++meshIndex;
        --remaining;
    } while (remaining != 0);

    self->field_0070 = 0;
    return reinterpret_cast<unsigned int**>(self);
}

HOOK(0x004CEC80, C_0083ae28_InitMissionInterface, "auto");
