#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t* __fastcall SceneRequest_Setup(
    uint32_t* self, void* /*edx*/, int requestId,
    uint32_t* position, int argument4, int argument5)
{
    CSF_AT(void*, self, 0x8) = CSF_VTBL(0x0082D448);
    CSF_AT(int32_t, self, 0xC) = -1;
    CSF_AT(int32_t, self, 0x10) = -1;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(int32_t, self, 0x4) = -1;
    CSF_AT(int32_t, self, 0x0) = requestId;
    CSF_AT(uint32_t, self, 0x14) = position[0];
    CSF_AT(uint32_t, self, 0x18) = position[1];
    CSF_AT(uint32_t, self, 0x1C) = position[2];
    CSF_AT(int32_t, self, 0x20) = argument5;
    CSF_AT(int32_t, self, 0x24) = argument4;
    return self;
}

HOOK(0x00446100, SceneRequest_Setup, "auto");
