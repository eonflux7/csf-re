#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPrtStdAtomicGetEmitter, 0x0057C290, (int a1));
CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __fastcall C_00862748_UpdateEmitterLifetime(
    C_00862748* self, void* /*edx*/, float deltaTime)
{
    const char globalFlag = self->vCheckGlobalObjectFlag();
    if (globalFlag)
        return globalFlag;

    auto* emitter = reinterpret_cast<uint8_t*>(
        RpPrtStdAtomicGetEmitter(static_cast<int>(self->field_0024)));

    const int standardOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, CSF_AT(void*, emitter, 0x0C), 0x10), 1);
    RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, CSF_AT(void*, emitter, 0x0C), 0x10), 9);

    uint8_t* standard = standardOffset == -1
        ? nullptr
        : emitter + standardOffset;

    const bool lifetimeEnabled = (self->field_0014 & 2u) == 0;
    const float elapsedTime = deltaTime + self->field_0040;
    self->field_0040 = elapsedTime;

    if (lifetimeEnabled && !self->field_003c && self->field_002c != 1)
    {
        auto* settings = reinterpret_cast<S_006dcf90*>(self->field_0030);
        if (*reinterpret_cast<const float*>(&settings->field_0014) != g_fZero &&
            elapsedTime + CSF_AT(float, standard, 0x08) >=
                *reinterpret_cast<const float*>(&settings->field_0014))
        {
            CSF_AT(int, standard, 0x44) = 0;
            self->field_003c = 1;
        }
    }

    char result = static_cast<char>(self->field_003c);
    if (result)
    {
        emitter = reinterpret_cast<uint8_t*>(
            RpPrtStdAtomicGetEmitter(static_cast<int>(self->field_0024)));

        if (self->field_002c == 1)
        {
            self->vSetFlag0C(1);
            result = 0;
        }
        else
        {
            auto* settings = reinterpret_cast<S_006dcf90*>(self->field_0030);
            if (*reinterpret_cast<const float*>(&settings->field_0090) +
                    *reinterpret_cast<const float*>(&settings->field_0018) <=
                self->field_0040)
            {
                self->vSetFlag0C(1);
                result = 0;
            }
            else
            {
                const int particleCount = CSF_AT(int, emitter, 0x10);
                result = static_cast<char>(particleCount);
                if (particleCount == 0)
                {
                    self->vSetFlag0C(1);
                    result = 0;
                }
            }
        }
    }

    self->field_0014 |= 8u;
    return result;
}

HOOK(0x006F3890, C_00862748_UpdateEmitterLifetime, "auto");
