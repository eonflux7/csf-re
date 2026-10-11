#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Audio_SetEffectOverrideEnabled, 0x007DF700,
         (int a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, Audio_SetEffectInt, 0x007E1380,
         (uint32_t* a1, int a2, int a3, ...));
CSF_FUNC(int, __cdecl, RpMTEffectGetInt, 0x007E13D0,
         (uint32_t* a1, int a2, int a3, uint32_t* a4));

static void __fastcall C_00863dd0_SynchronizeEffectValue(
    C_00863dd0* self, void* /*edx*/, char capture)
{
    auto* state = reinterpret_cast<uint32_t*>(self->field_0004);
    if (!CSF_AT(uint32_t, state, 0x50))
        return;

    if (capture)
    {
        const uint32_t flags = CSF_AT(uint32_t, state, 0x34);
        if ((flags & 2) == 0)
            return;

        auto* effect =
            reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, state, 0x30));

        uint32_t value;
        if (effect && (flags & 2) != 0)
            value = static_cast<uint32_t>(RpMTEffectGetInt(effect, 7, 2, nullptr));
        else
            value = CSF_AT(uint32_t, state, 0x48);

        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x48) = value;
        const int stateAddress = static_cast<int>(self->field_0004);
        self->field_000c = value;
        Audio_SetEffectOverrideEnabled(stateAddress, 0);
    }
    else
    {
        if ((CSF_AT(uint32_t, state, 0x34) & 2) != 0)
            return;

        CSF_AT(uint32_t, state, 0x48) = self->field_000c;
        auto* currentState = reinterpret_cast<void*>(self->field_0004);
        auto* effect = reinterpret_cast<uint32_t*>(
            CSF_AT(uint32_t, currentState, 0x30));

        if (effect)
            Audio_SetEffectInt(effect, 7, 1, self->field_000c);

        Audio_SetEffectOverrideEnabled(static_cast<int>(self->field_0004), 1);
    }
}

HOOK(0x007124D0, C_00863dd0_SynchronizeEffectValue, "auto");
