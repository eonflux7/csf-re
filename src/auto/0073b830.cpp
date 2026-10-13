#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static char __fastcall C_0086754c_ConfigureTransition(
    C_0086754c* self, void* /*edx*/, int settingsAddress)
{
    void* settings = reinterpret_cast<void*>(
        static_cast<uintptr_t>(settingsAddress));

    if (CSF_AT(uint8_t, settings, 0x11))
    {
        const uint8_t reversed = self->field_0018 == 0;
        self->field_0010 = self->field_0014 - self->field_0010;
        self->field_0018 = reversed;
        return static_cast<char>(reversed);
    }

    self->field_0014 = CSF_AT(float, settings, 0x18);
    self->field_0018 = CSF_AT(uint8_t, settings, 0x10);

    const uint32_t count = CSF_AT(uint32_t, settings, 0x0C);
    double numericCount = static_cast<double>(static_cast<int32_t>(count));
    self->field_0020 = count;
    if (static_cast<int32_t>(count) < 0)
        numericCount += g_UnsignedIntegerBias;

    const int64_t scaledCount =
        static_cast<int64_t>(numericCount * self->field_0014);
    const uint8_t direction = self->field_0018;
    self->field_0024 = static_cast<uint32_t>(scaledCount);

    const uint32_t initialCount =
        direction ? static_cast<uint32_t>(scaledCount) : 0;
    self->field_001c = initialCount;

    const float rate = CSF_AT(float, settings, 0x14);
    self->field_002c = rate;
    const float scaledRate = rate * self->field_0014;
    std::memcpy(&self->field_0030, &scaledRate, sizeof(scaledRate));

    self->field_0010 = 0.0f;
    self->field_0028 =
        direction ? scaledRate + g_TransitionOne : g_TransitionOne;

    return static_cast<char>(initialCount);
}

HOOK(0x0073B830, C_0086754c_ConfigureTransition, "auto");
