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

CSF_FUNC(void*, __fastcall, C_00833908_SetSceneCallback, 0x0049F560, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00833908_ConfigureMissionInterface, 0x0049F5A0, (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, TCharTask_BaseInit_4D0350, 0x004D0350, (void* self, void* edx, int a2));

// Initializes a C_0083af30 (TCharTask-derived scene task): runs the base init,
// installs its vtables, registers the scene callback and configures the
// mission interface slots, then clears field_0070.
static uint32_t** __fastcall C_0083af30_InitAndConfigureScene(C_0083af30* self, void* /*edx*/, int a2)
{
    TCharTask_BaseInit_4D0350(self, nullptr, a2);

    C_00833908* sceneInterface = reinterpret_cast<C_00833908*>(self->field_0014);
    self->vtbl = CSF_VTBL(0x0083AF30);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083AF10));

    C_00833908_SetSceneCallback(sceneInterface, nullptr, reinterpret_cast<void*>(2));
    C_00833908_ConfigureMissionInterface(sceneInterface, nullptr, 0, 1000, 1000, 1);
    C_00833908_ConfigureMissionInterface(sceneInterface, nullptr, 1, 1001, 1001, 1);

    int slotId = 1051;
    int remaining = 8;
    do
    {
        C_00833908_ConfigureMissionInterface(sceneInterface, nullptr, 2, slotId, slotId - 49, 0);
        ++slotId;
        --remaining;
    } while (remaining != 0);

    self->field_0070 = 0;
    return reinterpret_cast<uint32_t**>(self);
}

HOOK(0x004CEE00, C_0083af30_InitAndConfigureScene, "auto");
