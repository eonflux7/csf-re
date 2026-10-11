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

CSF_FUNC(int, __fastcall, QueryObjectCondition_48E210, 0x0048E210, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NotifyDepleted_49D400, 0x0049D400, (void* self, void* edx, void* a3, int a4));
CSF_FUNC(int, __fastcall, ApplyAmountToSubobject_4E1180, 0x004E1180, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

// Subtracts a2[4] from the counter at +0x08. If the counter drops to zero or below,
// it is clamped to zero and the owner is notified. Always forwards to the sub-object
// handler afterwards, unless the query on the owner at +0x0C reports a condition.
static int __fastcall C_0083e500_ConsumeAmountAndNotify(C_0083e500* self, void* /*edx*/, uint32_t* a2)
{
    int result = static_cast<int>(self->field_0008);
    if (result > 0)
    {
        result = QueryObjectCondition_48E210(reinterpret_cast<void*>(self->field_000c), nullptr);
        if ((result & 0xFF) == 0)
        {
            int32_t remaining = static_cast<int32_t>(self->field_0008) - static_cast<int32_t>(a2[4]);
            self->field_0008 = static_cast<uint32_t>(remaining);
            if (remaining <= 0)
            {
                self->field_0008 = 0;
                NotifyDepleted_49D400(reinterpret_cast<void*>(self->field_000c), nullptr,
                                      reinterpret_cast<void*>(&g_ZeroVectorX), 0);
            }
            return ApplyAmountToSubobject_4E1180(self, nullptr, a2);
        }
    }
    return result;
}

HOOK(0x004E18B0, C_0083e500_ConsumeAmountAndNotify, "auto");
