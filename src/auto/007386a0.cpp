#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AllocateAlignedBuffer, 0x00690EB0, (std::size_t size, int alignment, int option1, int option2));

// Sets up the per-entry buffer of this object (28 bytes per entry) and, when a
// positive second count is given, a secondary 2-byte-per-entry buffer.
// Returns nonzero when the primary buffer was allocated.
static int __fastcall C_00867358_InitBuffers(C_00867358* self, void* /*edx*/, int entryCount, int secondaryCount)
{
    self->field_0008 = static_cast<uint32_t>(entryCount);
    self->field_0004 = static_cast<uint32_t>(
        AllocateAlignedBuffer(static_cast<std::size_t>(28 * entryCount), 64, 0, 0));
    if (secondaryCount > 0)
    {
        self->field_0014 = static_cast<uint32_t>(secondaryCount);
        self->field_0010 = static_cast<uint32_t>(
            AllocateAlignedBuffer(static_cast<std::size_t>(2 * secondaryCount), 64, 0, 0));
    }
    return self->field_0004 != 0;
}

HOOK(0x7386A0, C_00867358_InitBuffers, "auto");
