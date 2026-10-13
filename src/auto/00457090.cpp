#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_456E10, 0x00456E10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Restores the C_008312e0 vtable, releases the optional sub-resource and the two
// heap buffers held at +0x20 and +0x10, then installs the final base vtable.
static int __fastcall C_008312e0_ReleaseHudPortrait(C_008312e0* self, void* /*edx*/)
{
    uint8_t hasSubResource = self->field_0008;

    self->vtbl = CSF_VTBL(0x008312E0);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x008312C4));

    if (hasSubResource) {
        sub_456E10(self, nullptr);
        self->field_0008 = 0;
    }

    if (self->field_0020) {
        Mem_Free(reinterpret_cast<void*>(self->field_0020));
        self->field_0020 = 0;
    }

    int result = static_cast<int>(self->field_0010);
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0010));
        self->field_0010 = 0;
    }

    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    return result;
}

HOOK(0x00457090, C_008312e0_ReleaseHudPortrait, "auto");
