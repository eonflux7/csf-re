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

// g_fZero: float constant 0.0f used for the facing test.
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// __thiscall(this, DWORD* a2, DWORD* a3), ret 8.
// Copies two endpoint vectors (3 dwords each) out of the object. The one whose
// slot is chosen depends on the sign of the float at +0x20: if <= 0, the
// (+0x34) vector goes to a2 and (+0x28) to a3; otherwise the reverse.
// Returns a3 in the first branch, or the dword loaded from +0x38 in the second.
static uint32_t* __fastcall CharActionFsm_GetFacingEndpoints(void* self, void* /*edx*/,
                                                             uint32_t* a2, uint32_t* a3) {
    uint32_t* result;

    if (CSF_AT(float, self, 0x20) <= g_fZero) {
        a2[0] = CSF_AT(uint32_t, self, 0x34);
        a2[1] = CSF_AT(uint32_t, self, 0x38);
        a2[2] = CSF_AT(uint32_t, self, 0x3C);

        result = a3;
        a3[0] = CSF_AT(uint32_t, self, 0x28);
        a3[1] = CSF_AT(uint32_t, self, 0x2C);
        a3[2] = CSF_AT(uint32_t, self, 0x30);
    } else {
        a2[0] = CSF_AT(uint32_t, self, 0x28);
        a2[1] = CSF_AT(uint32_t, self, 0x2C);
        a2[2] = CSF_AT(uint32_t, self, 0x30);

        a3[0] = CSF_AT(uint32_t, self, 0x34);
        result = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x38));
        a3[1] = reinterpret_cast<uint32_t>(result);
        a3[2] = CSF_AT(uint32_t, self, 0x3C);
    }

    return result;
}

HOOK(0x004094A0, CharActionFsm_GetFacingEndpoints, "auto");
