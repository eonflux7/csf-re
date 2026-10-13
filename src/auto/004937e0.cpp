#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_Destroy_48EC00, 0x0048EC00, (C_00833908* self, void*));

static int __fastcall C_00833cf8_Destroy(C_00833cf8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00833CF8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00833C78));
    CSF_AT(uint32_t, self, 0x008) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00833C74));
    self->field_000c = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00833C68));
    CSF_AT(uint32_t, self, 0x010) = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00833C60));
    self->field_0014 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00833C5C));
    return C_00833908_Destroy_48EC00(self, nullptr);
}

HOOK(0x004937E0, C_00833cf8_Destroy, "auto");
