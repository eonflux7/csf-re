#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AllocateAlignedBuffer, 0x00690EB0, (std::size_t size, int alignment, int option1, int option2));
CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x00690F70, (void* block));

// Frees the two owned buffers (index table at +0x10, element buffer at +0x04),
// then reallocates the element buffer for a2 entries of 28 bytes each and,
// if a3 > 0, a 2-byte-per-entry index buffer of a3 entries.
// Returns nonzero when the element buffer was allocated.
static int __fastcall C_00867358_ReallocateBuffers(C_00867358* self, void* /*edx*/, int entryCount, int indexCount)
{
    if (self->field_0010)
        FreeBlock_690F70(reinterpret_cast<void*>(self->field_0010));
    if (self->field_0004)
        FreeBlock_690F70(reinterpret_cast<void*>(self->field_0004));

    self->field_0008 = static_cast<uint32_t>(entryCount);
    self->field_0004 = static_cast<uint32_t>(
        AllocateAlignedBuffer(static_cast<std::size_t>(28 * entryCount), 64, 0, 0));
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;

    if (indexCount > 0)
    {
        self->field_0014 = static_cast<uint32_t>(indexCount);
        self->field_0010 = static_cast<uint32_t>(
            AllocateAlignedBuffer(static_cast<std::size_t>(2 * indexCount), 64, 0, 0));
    }

    return self->field_0004 != 0 ? 1 : 0;
}

HOOK(0x00738730, C_00867358_ReallocateBuffers, "auto");
