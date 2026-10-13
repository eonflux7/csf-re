#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees (bound by address, as given in the packet)
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));

static C_00861dd8* __fastcall C_00861dd8_Construct(C_00861dd8* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x00861DD8);
    GfxObject_ConstructEmbeddedState(reinterpret_cast<C_00860700*>(reinterpret_cast<uint8_t*>(self) + 0x0C), nullptr);
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 10;
    self->field_0008 = 0;
    self->field_0004 = static_cast<uint32_t>(-1);
    return self;
}

HOOK(0x006E7E60, C_00861dd8_Construct, "auto");
