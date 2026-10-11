#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Handle_GetValuePointer: __fastcall-style, ecx = handle object, edx unused.
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));

// Returns 1 and stores the handle's value pointer when the 0x400 flag is set in field_0008.
// Otherwise returns 0 and leaves field_002c unchanged.
static char __fastcall C_008656d0_CaptureHandleValueIfFlagged(C_008656d0* self, void* /*edx*/, C_00862490* a2)
{
    if ((self->field_0008 & 0x400) == 0)
        return 0;

    self->field_002c = static_cast<uint32_t>(Handle_GetValuePointer(a2, nullptr));
    return 1;
}

HOOK(0x006C5960, C_008656d0_CaptureHandleValueIfFlagged, "auto");
