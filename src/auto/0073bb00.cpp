#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TimerEpsilon, 0x867480);

static void __fastcall AdvanceLinearTimer(void* self, void* /*edx*/, float deltaTime)
{
    if (CSF_AT(uint8_t, self, 4) &&
        std::fabs(-CSF_AT(float, self, 0x14)) > g_TimerEpsilon)
    {
        void* target = CSF_AT(void*, self, 8);
        auto finishedCheck = reinterpret_cast<uint8_t(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, target, 0)[4]);
        if (finishedCheck(target, nullptr, CSF_AT(float, self, 0x10)))
        {
            CSF_AT(uint8_t, self, 4) = 0;
        }
        else
        {
            void* advanceTarget = CSF_AT(void*, self, 8);
            float newValue = deltaTime + CSF_AT(float, self, 0x10);
            CSF_AT(float, self, 0x10) = newValue;

            alignas(8) uint8_t stackBuf[0x10];
            auto advance = reinterpret_cast<void(__fastcall*)(void*, void*, uint8_t*, float)>(
                CSF_AT(void**, advanceTarget, 0)[5]);
            advance(advanceTarget, nullptr, stackBuf, newValue);
        }
    }
}

HOOK(0x0073BB00, AdvanceLinearTimer, "auto");
