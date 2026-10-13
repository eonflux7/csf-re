#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Points the vtable and the two mirror fields of C_008310f8 at the class's
// vtable (0x82D410), and when bit 0 of the flag argument is set, releases the
// object through the core memory heap. Returns the object pointer (eax = this).
static void* __fastcall C_008310f8_ResetVtableAndMaybeFree(C_008310f8* self, void* /*edx*/, char releaseFlag) {
    const auto vtable = CSF_VTBL(0x0082D410);
    const uint32_t vtableBits = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(vtable));

    self->field_0014 = vtableBits;
    self->field_0008 = vtableBits;
    self->vtbl = vtable;

    if ((releaseFlag & 1) != 0) {
        CoreMemory_Free(self);
    }
    return self;
}

HOOK(0x4018B0, C_008310f8_ResetVtableAndMaybeFree, "auto");
