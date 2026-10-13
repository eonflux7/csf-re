#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Copies the 6-dword bounds block referenced by this->field_0014 into a2.
// Returns false (and writes nothing) when the reference is null.
static bool __fastcall S_00402350_CopyBounds(S_00402350* self, void* /*edx*/, uint32_t* outBounds) {
    uint32_t* source = reinterpret_cast<uint32_t*>(self->field_0014);
    bool hasSource = source != nullptr;
    if (hasSource) {
        outBounds[0] = source[0];
        outBounds[1] = source[1];
        outBounds[2] = source[2];
        outBounds[3] = source[3];
        outBounds[4] = source[4];
        outBounds[5] = source[5];
    }
    return hasSource;
}

HOOK(0x006B80B0, S_00402350_CopyBounds, "auto");
