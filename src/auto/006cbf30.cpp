#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));

static int __fastcall C_00860bb8_Construct(C_00860bb8* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);

    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x008304D8);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860BB8);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x00860BB0);

    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 10;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 10;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 10;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 10;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 10;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 10;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_008c = 0;
    self->field_0090 = 10;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    self->field_00ac = 10;
    self->field_00c8 = 0;
    self->field_00cc = 0;
    self->field_00d0 = 0;
    self->field_00d4 = 10;
    self->field_00d8 = 0;
    self->field_00dc = 0;
    self->field_00e0 = 0;
    self->field_00e4 = 0;
    self->field_00e8 = 0;
    self->field_00ec = 0;
    self->field_00f0 = 0;

    self->field_00c0 = 1;
    self->field_0095 = 1;
    self->field_0096 = 1;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0064 = 0;
    self->field_0070 = 0;
    self->field_0010 = 0;
    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_00bc = 0;
    self->field_00c4 = 0;
    self->field_0068 = 0;
    self->field_0069 = 0;
    self->field_006a = 0;
    self->field_006b = 0;
    self->field_006c = 0;
    self->field_00b4 = 0;
    self->field_00f4 = 0;
    self->field_00b0 = -1;
    self->field_00b8 = -1;

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x006CBF30, C_00860bb8_Construct, "auto");
