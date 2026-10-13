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

// Base-object initializer (inherited ctor chain step).
CSF_FUNC(uint32_t*, __fastcall, ObjectBase_Initialize, 0x0064EBA0, (void* self, void* edx));

// Resets the DDL object manager embedded at this+0x44.
CSF_FUNC(int, __fastcall, NetDdlObjectManager_Reset, 0x00749E90, (char* self, void* edx));

// Returns the singleton self pointer of the C_008310b8 manager.
CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void* edx));

// Attaches an object to the registry under the given class id.
CSF_FUNC(int, __fastcall, Registry_Attach, 0x0065E310, (void* self, void* edx, int a2));

CSF_GLOBAL(uint32_t, g_DdlObjectClassId, 0x008BC180);

static int __fastcall C_00868768_Construct(C_00868768* self, void* /*edx*/)
{
    ObjectBase_Initialize(self, nullptr);
    self->vtbl = CSF_VTBL(0x00868768);

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(reinterpret_cast<uint8_t*>(self) + 0x44), nullptr);

    CSF_AT(uint8_t, self, 0x8C) = 0;

    NetDdlObjectManager_Reset(reinterpret_cast<char*>(self) + 0x44, nullptr);

    Registry_Attach(reinterpret_cast<void*>(&self->field_002c), nullptr, static_cast<int>(g_DdlObjectClassId));

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x0074F550, C_00868768_Construct, "auto");
