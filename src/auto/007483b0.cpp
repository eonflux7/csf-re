#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CMenuMpStats_RegisterMasterLoginNotification, 0x0074C6E0, (void* self, void* edx));

// Returns the result of Sub_64B250 when it is nonzero (tail-forwarding to the
// registration routine). Otherwise flags byte +0x332 and returns 0.
static char __fastcall CMenuMpStats_RegisterOrFlagPending(CMenuMpStats* self, void* /*edx*/)
{
    if (Sub_64B250(self, nullptr))
        return CMenuMpStats_RegisterMasterLoginNotification(self, nullptr);

    CSF_AT(uint8_t, self, 0x332) = 1;
    return 0;
}

HOOK(0x007483B0, CMenuMpStats_RegisterOrFlagPending, "auto");
