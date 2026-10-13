#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void* /*edx*/));

static int __fastcall NetProyectilDDL_InitializeObjectState(C_008310b8* self, void* /*edx*/) {
    C_008310b8_GetSelf(self, nullptr);
    CSF_AT(uint8_t, self, 0x24) = 0;
    CSF_AT(int32_t, self, 0x1C) = -1;
    CSF_AT(uint16_t, self, 2) = 0xFFFF;
    return (int)self;
}

HOOK(0x00747EE0, NetProyectilDDL_InitializeObjectState, "auto");
