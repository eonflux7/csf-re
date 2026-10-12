#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, ApplyBaseEffectParameter, 0x00708F40,
                (void* self, void* edx, int a2, int a3));

static int __fastcall C_00862a80_SetEffectParameter(
    C_00862a80* self, void* /*edx*/, int parameter, uint32_t* value)
{
    int result = ApplyBaseEffectParameter(
        self, nullptr, parameter,
        static_cast<int>(reinterpret_cast<uintptr_t>(value)));

    if (parameter == 0)
    {
        return self->vSlot1C(
            5, reinterpret_cast<uint32_t*>(
                   reinterpret_cast<uintptr_t>(value) * 255u));
    }

    if (parameter == 4)
    {
        self->field_0060 = 1;
        CSF_AT(uint32_t, self, 0x64) = value[0];
        CSF_AT(uint32_t, self, 0x68) = value[1];
        result = static_cast<int>(value[2]);
        CSF_AT(uint32_t, self, 0x6C) = static_cast<uint32_t>(result);
        CSF_AT(uint32_t, self, 0x70) = value[3];
    }

    return result;
}

HOOK(0x006F9280, C_00862a80_SetEffectParameter, "auto");
