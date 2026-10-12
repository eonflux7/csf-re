#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));

static int __fastcall C_008310b8_ResetSpawnMessage(C_008310b8* self, void* /*edx*/)
{
    C_008310b8_GetSelf(self, nullptr);
    CSF_AT(uint8_t, self, 0x0C) = 0;
    self->field_0008 = (self->field_0008 & 0x0000FFFFu) | 0xFFFF0000u;
    self->field_0008 = (self->field_0008 & 0xFFFF0000u) | 0x0000FFFFu;
    self->field_0004 = 0xFFFFFFFFu;
    CSF_AT(uint8_t, self, 0x00) = 0xFF;
    return reinterpret_cast<int>(self);
}

HOOK(0x00751AF0, C_008310b8_ResetSpawnMessage, "auto");
