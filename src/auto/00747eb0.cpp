#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));

// Resets the event descriptor's state fields to their "unset" values
// (-1 / 0xFFFF markers) and returns the object itself.
static C_008310b8* __fastcall C_008310b8_InitializeEventDescriptor(C_008310b8* self, void* /*edx*/)
{
    C_008310b8_GetSelf(self, nullptr);

    CSF_AT(uint8_t, self, 0x28) = 0;
    self->field_0004 = 0xFFFFFFFFu;
    CSF_AT(int, self, 0x20) = -1;
    CSF_AT(uint32_t, self, 0) = 0xFFFFFFFFu;
    CSF_AT(int, self, 0x24) = -1;

    return self;
}

HOOK(0x747EB0, C_008310b8_InitializeEventDescriptor, "auto");
