#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, ObjectBase_Initialize, 0x0064EBA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_InitializeEventDescriptor, 0x00747EB0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Registry_Attach, 0x0065E310, (void* self, void* edx, int a2));

CSF_GLOBAL(uint32_t, g_NetProyectilClassId, 0x008BC160);

static int* __fastcall C_00867770_Construct(C_00867770* self, void* /*edx*/)
{
    ObjectBase_Initialize(self, nullptr);
    *(void***)self = CSF_VTBL(0x00867770);
    MpSpawnPoints_InitializeEventDescriptor(reinterpret_cast<uint8_t*>(self) + 0x44, nullptr);
    Registry_Attach(reinterpret_cast<void*>(&self->field_002c), nullptr, (int)g_NetProyectilClassId);
    return reinterpret_cast<int*>(self);
}

HOOK(0x00747570, C_00867770_Construct, "auto");
