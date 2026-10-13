#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Initializes the TCharTask base sub-state: sets the base vtable and the
// placeholder/field slots at +0x04..+0x10, then returns the object pointer.
static uint32_t* __fastcall TCharTask_InitializeBase(TCharTask* self, void* /*edx*/) {
    CSF_AT(uint32_t, self, 0x04) = (uint32_t)CSF_ADDR_DATA(0x0082D428);
    self->vtbl = CSF_VTBL(0x0083DE60);
    CSF_AT(uint32_t, self, 0x04) = (uint32_t)CSF_ADDR_DATA(0x0083DE40);
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004DB4C0, TCharTask_InitializeBase, "auto");
