#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address: the original's first call (ecx = this, edx unused, no stack args).
CSF_FUNC(void, __fastcall, PrepareClumpForProcessing, 0x006FDE00, (void* self, void* edx));

// Bound by address: __cdecl, one stack argument (the clump), caller pops it.
CSF_FUNC(int, __cdecl, GfxFrames_ProcessClumpAtomics, 0x006C04E0, (int clump));

// Processes the clump stored at +0x08 if one is present; otherwise returns the (zero) field.
// __thiscall with one stack arg (a2, unused) -> callee pops 4 bytes.
static int __fastcall S_006fde00_ProcessClumpIfPresent(S_006fde00* self, void* /*edx*/, int /*a2*/) {
    int result = static_cast<int>(self->field_0008);
    if (result != 0) {
        PrepareClumpForProcessing(self, nullptr);
        return GfxFrames_ProcessClumpAtomics(static_cast<int>(self->field_0008));
    }
    return result;
}

HOOK(0x006FDEC0, S_006fde00_ProcessClumpIfPresent, "auto");
