#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee bindings (by address)
CSF_FUNC(int, __cdecl, Sub_7A8ED0, 0x007A8ED0, (int argument));
CSF_FUNC(void*, __fastcall, CMenuIntroVideo_RefreshState, 0x00535900, (void* self, void* edx));

// Hides the pause overlay unless it is in state 1, or an attached child
// reports a blocking state (checked via the callee at 0x7A8ED0).
static void __fastcall MenuMpPause_TryHideOverlay(void* self, void* /*edx*/, int a2, int a3)
{
    (void)a3; // Argument is popped by the callee convention but unused.

    if (CSF_AT(int, self, 0x10) == 1)
        return;

    uint8_t* child = CSF_AT(uint8_t*, self, 0x20);
    if (child && CSF_AT(uint8_t, child, 0x11) != 0 &&
        !Sub_7A8ED0(a2))
        return;

    CSF_AT(uint8_t, self, 0x25) = 0;
    CMenuIntroVideo_RefreshState(static_cast<uint8_t*>(self) - 4, nullptr);
}

HOOK(0x00535C00, MenuMpPause_TryHideOverlay, "auto");
