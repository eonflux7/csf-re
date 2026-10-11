#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Audio_SetEffectActive, 0x007DF700, (int a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, Audio_SetEffectParameter, 0x007E1380, (uint32_t* a1, int a2, int a3, ...));
CSF_FUNC(int, __cdecl, RpMTEffectGetInt, 0x007E13D0, (uint32_t* a1, int a2, int a3, uint32_t* a4));

static void __fastcall C_00863dd0_ClearPendingEffectState(
    C_00863dd0* self, void* /*edx*/)
{
    auto* state = reinterpret_cast<uint8_t*>(self->field_0004);
    if (!CSF_AT(uint32_t, state, 0x50))
        return;

    const uint32_t flags = CSF_AT(uint32_t, state, 0x34);
    if (!(flags & 2) || !(flags & 4))
        return;

    auto* effect = CSF_AT(uint32_t*, state, 0x30);
    const int parameter = effect && (flags & 2)
        ? RpMTEffectGetInt(effect, 7, 2, nullptr)
        : CSF_AT(int, state, 0x48);

    CSF_AT(int, reinterpret_cast<void*>(self->field_0004), 0x48) = parameter;
    Audio_SetEffectActive(static_cast<int>(self->field_0004), 0);

    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x34) &= ~4u;
    effect = CSF_AT(uint32_t*, reinterpret_cast<void*>(self->field_0004), 0x30);
    if (effect)
        Audio_SetEffectParameter(effect, 8, 1, 0);

    CSF_AT(int, reinterpret_cast<void*>(self->field_0004), 0x48) = parameter;
    state = reinterpret_cast<uint8_t*>(self->field_0004);
    if (CSF_AT(uint32_t*, state, 0x30))
        Audio_SetEffectParameter(CSF_AT(uint32_t*, state, 0x30), 7, 1, parameter);

    Audio_SetEffectActive(static_cast<int>(self->field_0004), 1);
}

HOOK(0x00712410, C_00863dd0_ClearPendingEffectState, "auto");
