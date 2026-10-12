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

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Virtual slot 2 (offset 0x08): returns a double (x87 st0) in the original.
using GfxVirtGetDouble = double(__fastcall*)(void* self, void* edx);
// Virtual slot 1 (offset 0x04): takes one float argument on the stack.
using GfxVirtSetFloat = void(__fastcall*)(void* self, void* edx, float value);

static void __fastcall GfxObject_UpdateAttachment(float* self, void* /*edx*/, float deltaTime)
{
    // self[4] is the remaining time (offset 0x10), self[3] the target value (offset 0x0C).
    if (self[4] > g_fZero)
    {
        float remaining = self[4] - deltaTime;
        self[4] = remaining;

        if (remaining > g_fZero)
        {
            double currentValue = ((GfxVirtGetDouble)CSF_AT(void**, self, 0)[2])(self, nullptr);
            float interpolated = (self[3] - (float)currentValue) / (deltaTime + self[4]) * deltaTime
                               + (float)currentValue;
            ((GfxVirtSetFloat)CSF_AT(void**, self, 0)[1])(self, nullptr, interpolated);
        }
        else
        {
            float finalValue = self[3];
            self[4] = 0.0f;
            ((GfxVirtSetFloat)CSF_AT(void**, self, 0)[1])(self, nullptr, finalValue);
        }
    }
}

HOOK(0x006E50C0, GfxObject_UpdateAttachment, "auto");
