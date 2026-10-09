#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Dispatches on the request code stored at *request (0, 1, 2 select
// the three virtual handlers; any other code returns (code - 2)).
static int __fastcall CCharWeaponFsm_DispatchRequest(CCharWeaponFsm* self, void* /*edx*/, uint32_t* request)
{
    // The original reads *request once; keep a single load.
    const uint32_t requestCode = *request;

    if (requestCode == 0)
        return self->vSlot124(reinterpret_cast<int>(request));

    if (requestCode == 1)
        return self->vSlot128(reinterpret_cast<int>(request));

    if (requestCode == 2)
        return self->vSlot12C();

    // Fallthrough: the original leaves eax = code - 2 as the return value.
    return static_cast<int>(requestCode - 2u);
}

HOOK(0x4809F0, CCharWeaponFsm_DispatchRequest, "auto");
