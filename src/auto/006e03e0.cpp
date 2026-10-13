#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Virtual slot 0x08 of C_008619e0, called with no arguments before the stores.
using C_008619e0_ConsumeFlagFn = void(__fastcall*)(void* self, void* edx);

static char __fastcall C_008619e0_ConsumeFlagAndSetValue(C_008619e0* self, void* /*edx*/,
                                                         int /*a2*/, int a3, int /*a4*/) {
    // vConsumeFlag0008(this)
    reinterpret_cast<C_008619e0_ConsumeFlagFn>(CSF_AT(void**, self, 0)[2])(self, nullptr);

    // this->field_0004 = a3; this->field_0008 = 1; return 1;
    CSF_AT(int, self, 0x4) = a3;
    CSF_AT(uint8_t, self, 0x8) = 1;
    return 1;
}

HOOK(0x006E03E0, C_008619e0_ConsumeFlagAndSetValue, "auto");
