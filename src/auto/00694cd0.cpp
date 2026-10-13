#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Grows the owned format buffer in 20 KiB steps until it can hold `extraBytes`
// more bytes past the current used length. Returns used + extraBytes.
static unsigned int __fastcall StringFormat_EnsureBufferCapacity(S_00694cd0* self, void* /*edx*/, int extraBytes)
{
    unsigned int required = static_cast<unsigned int>(extraBytes) + self->field_000c;

    while (required >= self->field_0008) {
        uint32_t newCapacity = self->field_0008 + 0x5000;
        self->field_0008 = newCapacity;

        uint32_t newBlock = static_cast<uint32_t>(CoreMemory_AllocUntagged(newCapacity));
        std::memcpy(reinterpret_cast<void*>(newBlock),
                    reinterpret_cast<const void*>(self->field_0000),
                    self->field_000c);

        if (self->field_0000) {
            CoreMemory_Free(reinterpret_cast<void*>(self->field_0000));
            self->field_0000 = 0;
        }
        self->field_0000 = newBlock;

        required = static_cast<unsigned int>(extraBytes) + self->field_000c;
    }
    return required;
}

HOOK(0x00694CD0, StringFormat_EnsureBufferCapacity, "auto");
