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

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* edx));

static int __fastcall GfxObject_ConstructEmbeddedState(C_00860700* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);

    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082D410));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082D428));
    self->vtbl = CSF_VTBL(0x00860700);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008606F8));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008606DC));

    self->field_0014 = 0.0f;
    self->field_0018 = 0.0f;
    self->field_001c = 0.0f;
    self->field_000c = 0;
    self->field_0010 = 0;

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x006BFD00, GfxObject_ConstructEmbeddedState, "auto");
