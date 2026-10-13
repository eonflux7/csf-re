#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

// Constructor for C_0085e7b8: sets the vtable, allocates a 256-byte untagged
// buffer, records its capacity (0x100 on success, 0 on failure), and zeroes
// the two trailing fields. Returns this.
static C_0085e7b8* __fastcall C_0085e7b8_Construct(C_0085e7b8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0085E7B8);

    int allocatedBuffer = CoreMemory_AllocUntagged(256);
    self->field_0004 = static_cast<uint32_t>(allocatedBuffer);
    self->field_0010 = (allocatedBuffer != 0) ? 0x100u : 0u;

    self->field_0008 = 0;
    self->field_000c = 0;
    return self;
}

HOOK(0x0069A590, C_0085e7b8_Construct, "auto");
