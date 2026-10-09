#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, Hud_IsSuspended, 0x00495520, (void* self, void* edx));

static int __fastcall CCharActionFsm_ResolveActiveItemHandle(CCharActionFsm* self, void* /*edx*/)
{
    if (self->field_0084 && Hud_IsSuspended(reinterpret_cast<void*>(self->field_0008), nullptr))
        return static_cast<int>(self->field_0084);

    uint32_t slotIndex = self->field_0090;
    int result = 0;
    if (slotIndex != 0xFFFFFFFFu)
        return *reinterpret_cast<int*>(self->field_0078 + 4 * slotIndex);
    return result;
}

HOOK(0x485B70, CCharActionFsm_ResolveActiveItemHandle, "auto");
