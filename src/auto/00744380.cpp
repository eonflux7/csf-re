#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));

// Clears every field of a C_008310b8 net callback object and returns it.
static int __fastcall C_008310b8_ClearState(C_008310b8* self, void* /*edx*/) {
    C_008310b8_GetSelf(self, nullptr);

    CSF_AT(uint8_t, self, 0x4C) = 0;
    self->field_0008 = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;
    self->field_0004 = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint8_t, self, 0) = 0;

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x00744380, C_008310b8_ClearState, "auto");
