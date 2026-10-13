#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, NetPlayerDdl_InitializeBaseDescriptor, 0x007A2DC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Registry_Attach, 0x0065E310, (void* self, void* edx, int a2));

CSF_GLOBAL(uint32_t, g_DdlObjectClassId, 0x008BC1CC);

// Initializes the C_00868050 object: runs the base descriptor setup, installs
// C_00868050's vtable, and attaches the object's registry slot (+0x2C) using the
// DDL object class id. Returns the object itself.
static int* __fastcall C_00868050_InitializeAndAttachRegistry(C_00868050* self, void* /*edx*/)
{
    NetPlayerDdl_InitializeBaseDescriptor(self, nullptr);
    self->vtbl = CSF_VTBL(0x00868050);
    Registry_Attach(&self->field_002c, nullptr, static_cast<int>(g_DdlObjectClassId));
    return reinterpret_cast<int*>(self);
}

HOOK(0x00750CE0, C_00868050_InitializeAndAttachRegistry, "auto");
