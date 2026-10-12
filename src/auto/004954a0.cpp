#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);

static double __fastcall CPlayerCharacter_GetContextHeightOffset(CPlayerCharacter* self, void* /*edx*/)
{
    alignas(8) uint8_t valuesBuffer[12];

    // Virtual slot 0xA4 (index 41) fills the buffer and returns a float array; the height is element [1].
    float* values = reinterpret_cast<float* (__fastcall*)(void*, void*, void*)>(CSF_AT(void**, self, 0)[41])(
        self, nullptr, valuesBuffer);

    double offset = (double)values[1];
    if (self->field_0228)
        offset = offset - self->field_0234;

    return self->field_03d4 - offset + g_fReturnedConstant_0082E030;
}

HOOK(0x004954A0, CPlayerCharacter_GetContextHeightOffset, "auto");
