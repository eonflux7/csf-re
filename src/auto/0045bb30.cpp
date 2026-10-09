#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Unnamed callee at 0x755880; signature bound as specified in the packet.
CSF_FUNC(int, __cdecl, sub_755880, 0x00755880, (int a1));

// Returns true when a2 is non-null and the low byte of sub_755880(a2->field_0050) is nonzero.
static bool __stdcall IsObjectSubFieldFlagSet(int /*a1*/, int a2) {
    if (a2 == 0) {
        return false;
    }
    int subFieldValue = CSF_AT(int, reinterpret_cast<void*>(a2), 0x50);
    return (uint8_t)sub_755880(subFieldValue) != 0;
}

HOOK(0x0045BB30, IsObjectSubFieldFlagSet, "auto");
