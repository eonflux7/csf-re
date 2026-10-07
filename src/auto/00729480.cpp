#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00865aa8_UpdateBaseState, 0x00713EF0,
         (C_00865aa8* self, void* edx, int elapsedTimeBits));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fOscillationPi, 0x008656AC);

static void __fastcall C_00865aa8_AdvanceOscillation(
    C_00865aa8* self, void* /*edx*/, int elapsedTimeBits)
{
    C_00865aa8_UpdateBaseState(self, nullptr, elapsedTimeBits);

    void* linkedObject =
        ((void* (__fastcall*)(void*, void*))self->vtbl[6])(
            self, nullptr);

    if (linkedObject == nullptr)
        return;

    const std::uint8_t status =
        ((std::uint8_t (__fastcall*)(void*, void*))
            CSF_AT(void**, linkedObject, 0)[47])(
                linkedObject, nullptr);

    if (status == 0)
        return;

    const float zero = g_fZero;
    if (CSF_AT(float, linkedObject, 0x118) == zero)
        return;

    const double duration = CSF_AT(float, linkedObject, 0x118);

    float elapsedTime;
    std::memcpy(&elapsedTime, &elapsedTimeBits, sizeof(elapsedTime));

    double accumulatedTime =
        static_cast<double>(elapsedTime) +
        static_cast<double>(self->field_0054);

    static_cast<volatile float&>(self->field_0054) =
        static_cast<float>(accumulatedTime);

    if (accumulatedTime >= duration)
        accumulatedTime = g_fZero;

    static_cast<volatile float&>(self->field_0054) =
        static_cast<float>(accumulatedTime);

    const double phase =
        (accumulatedTime / duration) *
        static_cast<double>(g_fOscillationPi);
    const double sine = std::sin(phase);

    const double upperValue = self->field_0060;
    const double difference =
        upperValue - static_cast<double>(self->field_005c);
    const double scaledSine = sine * difference;

    self->field_0058 = static_cast<float>(
        scaledSine + static_cast<double>(self->field_005c));
}

HOOK(0x00729480, C_00865aa8_AdvanceOscillation, "auto");
