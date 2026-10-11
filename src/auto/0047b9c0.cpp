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

// Callees (bound by original address)
CSF_FUNC(void, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Runs the base update, then picks the fire task or the fallback task
// depending on whether the owner's weapon action is blocked.
static uint32_t __fastcall TCharTask_DispatchFireOrFallback(void* self, void* /*edx*/, int a2)
{
    TCharTask_BaseUpdate(self, nullptr, a2);

    // owner = this->field_0008; actionFsm = *(owner + 0x14)
    uint32_t owner = CSF_AT(uint32_t, self, 0x08);
    uint32_t actionFsm = CSF_AT(uint32_t, reinterpret_cast<void*>(owner), 0x14);

    if (CharActionFsm_IsWeaponActionBlocked(reinterpret_cast<void*>(actionFsm), nullptr))
    {
        int offset = static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 80);
        return static_cast<uint32_t>(CharTask_LookupByOffset(self, nullptr, offset));
    }

    uint32_t result = CSF_AT(uint32_t, self, 0x08);
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x2EC))
    {
        int offset = static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 40);
        return static_cast<uint32_t>(CharTask_LookupByOffset(self, nullptr, offset));
    }
    return result;
}

HOOK(0x47B9C0, TCharTask_DispatchFireOrFallback, "auto");
