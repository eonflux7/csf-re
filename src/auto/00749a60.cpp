#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));

// Resets a spawn descriptor: clears the byte flag at +0x8, sets +0x4 to -1,
// marks the vtable slot byte at offset 0 as 0xFF, and returns the object.
static int __fastcall SpawnDescriptor_Initialize(C_008310b8* self, void* /*edx*/)
{
    C_008310b8_GetSelf(self, nullptr);

    *reinterpret_cast<uint8_t*>(&self->field_0008) = 0;
    self->field_0004 = 0xFFFFFFFFu;
    CSF_AT(uint8_t, self, 0) = 0xFF;

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x00749A60, SpawnDescriptor_Initialize, "auto");
