#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constructs a memory-stream object in place: stores the data pointer (a2),
// the vtable, the capacity/size argument (a3), and clears the two trailing
// fields. Returns the object itself (its vtable slot sits at offset 0).
static C_0085e7b8* __fastcall MemoryStream_Construct(C_0085e7b8* self, void* /*edx*/, uint32_t dataPointer, uint32_t sizeArg) {
    self->field_0004 = dataPointer;
    self->vtbl = CSF_VTBL(0x0085E7B8);
    self->field_0010 = sizeArg;
    self->field_0008 = 0;
    self->field_000c = 0;
    return self;
}

HOOK(0x0069A3F0, MemoryStream_Construct, "auto");
