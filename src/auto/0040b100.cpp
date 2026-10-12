#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, S0040b040_CheckPrimary, 0x0040B040, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, S0040b040_CheckSecondary, 0x0040B0A0, (void* self, void* edx));

// Vtable of S_0040b040 as listed by the packet. Not stored or used by this
// function's code path; kept so the binding is present.
[[maybe_unused]] static const void* const kS0040b040Vtable = CSF_VTBL(0x0040B040);

// Returns 1 only if both checks pass. The second check runs only when the first succeeds.
static int __fastcall S0040b040_PassesBothChecks(S_0040b040* self, void* /*edx*/)
{
    if (!S0040b040_CheckPrimary(self, nullptr)) {
        return 0;
    }
    if (!S0040b040_CheckSecondary(self, nullptr)) {
        return 0;
    }
    return 1;
}

HOOK(0x0040B100, S0040b040_PassesBothChecks, "auto");
