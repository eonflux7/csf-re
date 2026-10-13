#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Releases the portrait FLI/texture resources held by this object.
CSF_FUNC(int, __fastcall, Portrait_ReleaseFliResources, 0x00456E10, (void* self, void* edx));

// If the pending flag is set, releases the portrait resources and clears the flag.
// Returns the flag value read on entry, or the release routine's result (low byte) when the flag was set.
static char __fastcall Portraits_ResetPendingFlag(C_008312e0* self, void* /*edx*/) {
    char result = static_cast<char>(self->field_0008);
    if (result) {
        result = static_cast<char>(Portrait_ReleaseFliResources(self, nullptr));
        self->field_0008 = 0;
    }
    return result;
}

HOOK(0x457000, Portraits_ResetPendingFlag, "auto");
