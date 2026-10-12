#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Releases the clump loader's stream buffer state owned by this object.
CSF_FUNC(char, __fastcall, ClumpLoader_ReleaseStreamBuffer, 0x006FE470, (void* self, void* edx));

// If the pending-release flag is set, releases the stream buffer and clears the flag.
// Returns the flag value read on entry, or the release routine's result when the flag was set.
static char __fastcall C_00863018_ReleasePendingStream(C_00863018* self, void* /*edx*/) {
    char result = static_cast<char>(self->field_0021);
    if (result) {
        result = ClumpLoader_ReleaseStreamBuffer(self, nullptr);
    }
    self->field_0020 = 0;
    return result;
}

HOOK(0x6FE820, C_00863018_ReleasePendingStream, "auto");
