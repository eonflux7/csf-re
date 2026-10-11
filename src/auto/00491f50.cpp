#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, DestroyBaseObject, 0x0048EC00, (void* self, void* edx));

static int __fastcall C_00833a48_Destroy(C_00833a48* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00833A48);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008339C8));
    CSF_AT(uint32_t, self, 0x008) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008339C4));
    self->field_000c = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008339B8));
    CSF_AT(uint32_t, self, 0x010) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008339B0));
    self->field_0014 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008339AC));
    return DestroyBaseObject(self, nullptr);
}

HOOK(0x00491F50, C_00833a48_Destroy, "auto");
