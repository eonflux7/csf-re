#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_00833908_Construct, 0x00490940, (void* self, void* edx, int a2, int a3));

static uint32_t* __fastcall C_00833a48_Construct(
    C_00833a48* self, void* /*edx*/, int a2, int a3)
{
    C_00833908_Construct(self, nullptr, a2, a3);
    self->vtbl = CSF_VTBL(0x00833A48);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008339C8));
    CSF_AT(uint32_t, self, 0x008) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008339C4));
    self->field_000c = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008339B8));
    CSF_AT(uint32_t, self, 0x010) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008339B0));
    self->field_0014 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008339AC));
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00491EC0, C_00833a48_Construct, "auto");
