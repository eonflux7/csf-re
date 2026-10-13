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

CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(float, g_LevelScaleFactor, 0x0082E048);

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, BddLoader_ReleasePendingHandle, 0x0050E250, (void* self, void*));
CSF_FUNC(void, __fastcall, C_0083bc28_PrepareFinish, 0x004D3870, (void* self, void* edx));

static void __fastcall C_0083bc28_AdvanceTimer(C_0083bc28* self, void* /*edx*/, float deltaTime)
{
    float scaledPercent = self->field_0040 * g_SoundActivityThreshold * g_MaxPercent;
    void* globalState = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
    CSF_AT(float, globalState, 0xFC) = scaledPercent;

    self->field_0040 = self->field_0040 + deltaTime;
    if (self->field_0040 >= g_LevelScaleFactor)
    {
        uint32_t flags = (self->field_0044 & 0xFFFFFFF3u) | 8u;
        self->field_0044 = flags;
        if ((flags & 0x10u) == 0)
        {
            if (self->field_004c == 5)
            {
                void* state = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
                BddLoader_ReleasePendingHandle(state, nullptr);
            }
            C_0083bc28_PrepareFinish(self, nullptr);
            self->field_004c = 2;
        }
    }
}

HOOK(0x004D45F0, C_0083bc28_AdvanceTimer, "auto");
