#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008670f0_ApplyScaledValueAndRestore(
    C_008670f0* self, void* /*edx*/)
{
    alignas(8) uint8_t originalBytes[0x10];
    alignas(8) uint8_t scaledBytes[0x10];

    const float scaledValue = self->field_0100 * self->field_00b4;

    CSF_AT(uint32_t, originalBytes, 0x0) = self->field_00a8;
    CSF_AT(uint32_t, originalBytes, 0x4) = self->field_00ac;
    CSF_AT(float, scaledBytes, 0xC) = scaledValue;
    CSF_AT(uint32_t, originalBytes, 0x8) = self->field_00b0;
    CSF_AT(float, originalBytes, 0xC) = self->field_00b4;

    CSF_AT(uint32_t, scaledBytes, 0x0) = self->field_00a8;
    CSF_AT(uint32_t, scaledBytes, 0x4) = self->field_00ac;
    CSF_AT(uint32_t, scaledBytes, 0x8) = self->field_00b0;

    self->vSlot60(reinterpret_cast<float*>(scaledBytes));
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[8 / 4])(self, nullptr);
    return self->vSlot60(reinterpret_cast<float*>(originalBytes));
}

HOOK(0x007332A0, C_008670f0_ApplyScaledValueAndRestore, "auto");
