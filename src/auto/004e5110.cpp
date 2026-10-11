#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Stores the C_0083e5e8 vtable (and the stacked base-class vtable pointers) into
// the object. Returns the final vtable address, as the original does in eax.
static void* __fastcall C_0083e5e8_InitVtables(C_0083e5e8* self, void* /*edx*/) {
    // Original: eax = 0x82d410
    const uint32_t derivedVtblValue = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));

    // this->field_0020 = &off_82D410;
    CSF_AT(uint32_t, self, 0x20) = derivedVtblValue;

    // this->field_0004 = &off_83E144;  (base-class data pointer, overwritten below)
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0083E144)));

    // this->field_0004 = &off_82D410;
    self->field_0004 = derivedVtblValue;

    // this->__vftable = &off_82D410;
    self->vtbl = CSF_VTBL(0x0082D410);

    return reinterpret_cast<void*>(static_cast<uintptr_t>(derivedVtblValue));
}

HOOK(0x4E5110, C_0083e5e8_InitVtables, "auto");
