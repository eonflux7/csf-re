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

CSF_FUNC(int, __fastcall, sub_4E5F50, 0x004E5F50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4E65A0, 0x004E65A0, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_0083e9d0_TickCountdownAndDispatch(C_0083e9d0* self, void* /*edx*/)
{
    if (self->field_004c && self->field_0048 <= g_fZero)
    {
        uint32_t stageIndex = self->field_0030 - 1;
        self->field_0048 = 0.5f;
        if (stageIndex == 0)
        {
            sub_4E5F50(self, nullptr);
        }
        else if (stageIndex == 1)
        {
            sub_4E65A0(self, nullptr);
        }
    }
}

HOOK(0x004E6710, C_0083e9d0_TickCountdownAndDispatch, "auto");
