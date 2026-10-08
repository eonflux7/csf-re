#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CMenuMessageBox_UpdateHorizontalAnimation(
    CMenuMessageBox* self, void* /*edx*/, int x)
{
    using GetValues = std::uint32_t* (__fastcall*)(void*, void*);
    using SetValues = void (__fastcall*)(void*, void*, std::uint32_t*);
    using GetObject = void* (__fastcall*)(void*, void*);
    using SetValuesWithResult = int (__fastcall*)(void*, void*, std::uint32_t*);

    void* control = reinterpret_cast<void*>(self->field_005c);
    std::uint32_t* values =
        reinterpret_cast<GetValues>(CSF_AT(void**, control, 0)[0x9c / 4])(
            control, nullptr);

    control = reinterpret_cast<void*>(self->field_005c);
    const GetValues getBoundsSize =
        reinterpret_cast<GetValues>(CSF_AT(void**, control, 0)[0xa4 / 4]);
    const std::uint32_t minimumXBits = CSF_AT(std::uint32_t, values, 0);
    values = getBoundsSize(control, nullptr);

    std::uint32_t maximumXBits = CSF_AT(std::uint32_t, values, 0);
    maximumXBits -= self->field_09a0;
    maximumXBits += minimumXBits;

    std::uint32_t firstPosition[2];
    std::uint32_t secondPosition[2];
    std::uint32_t thirdPosition[2];

    control = reinterpret_cast<void*>(self->field_0068);
    values = reinterpret_cast<GetValues>(
        CSF_AT(void**, control, 0)[0x9c / 4])(control, nullptr);
    firstPosition[0] = CSF_AT(std::uint32_t, values, 0);
    firstPosition[1] = CSF_AT(std::uint32_t, values, 4);

    control = reinterpret_cast<void*>(self->field_0064);
    values = reinterpret_cast<GetValues>(
        CSF_AT(void**, control, 0)[0x9c / 4])(control, nullptr);
    secondPosition[0] = CSF_AT(std::uint32_t, values, 0);
    secondPosition[1] = CSF_AT(std::uint32_t, values, 4);

    control = reinterpret_cast<void*>(self->field_0060);
    values = reinterpret_cast<GetValues>(
        CSF_AT(void**, control, 0)[0x9c / 4])(control, nullptr);
    thirdPosition[0] = CSF_AT(std::uint32_t, values, 0);
    thirdPosition[1] = CSF_AT(std::uint32_t, values, 4);

    // Bounds comparisons are signed; coordinate arithmetic wraps at 32 bits.
    const bool outsideBounds =
        x > static_cast<std::int32_t>(maximumXBits) ||
        x < static_cast<std::int32_t>(minimumXBits);
    const std::uint32_t step = self->field_09a0;

    if (outsideBounds) {
        firstPosition[0] -= step;
        thirdPosition[0] += step;
        thirdPosition[0] += 2u * step;
        secondPosition[0] += step;
    } else {
        secondPosition[0] += step;
        thirdPosition[0] += step;
        firstPosition[0] += step;
    }

    control = reinterpret_cast<void*>(self->field_0068);
    reinterpret_cast<SetValues>(CSF_AT(void**, control, 0)[0x98 / 4])(
        control, nullptr, firstPosition);

    control = reinterpret_cast<void*>(self->field_0064);
    reinterpret_cast<SetValues>(CSF_AT(void**, control, 0)[0x98 / 4])(
        control, nullptr, secondPosition);

    control = reinterpret_cast<void*>(self->field_0060);
    reinterpret_cast<SetValues>(CSF_AT(void**, control, 0)[0x98 / 4])(
        control, nullptr, thirdPosition);

    if (outsideBounds) {
        self->field_09a0 = 0u - self->field_09a0;
    }

    std::uint32_t appearance[4];

    control = reinterpret_cast<void*>(self->field_0064);
    void* appearanceObject = reinterpret_cast<GetObject>(
        CSF_AT(void**, control, 0)[0x110 / 4])(control, nullptr);
    values = reinterpret_cast<GetValues>(
        CSF_AT(void**, appearanceObject, 0)[0x64 / 4])(
            appearanceObject, nullptr);

    appearance[0] = CSF_AT(std::uint32_t, values, 0);
    appearance[1] = CSF_AT(std::uint32_t, values, 4);
    appearance[2] = CSF_AT(std::uint32_t, values, 8);
    // Preserve the original read even though this component is overwritten.
    appearance[3] = CSF_AT(volatile std::uint32_t, values, 0xc);
    appearance[3] = 0x43480000u; // 200.0f

    reinterpret_cast<SetValues>(
        CSF_AT(void**, appearanceObject, 0)[0x60 / 4])(
            appearanceObject, nullptr, appearance);

    control = reinterpret_cast<void*>(self->field_0060);
    appearanceObject = reinterpret_cast<GetObject>(
        CSF_AT(void**, control, 0)[0x110 / 4])(control, nullptr);
    values = reinterpret_cast<GetValues>(
        CSF_AT(void**, appearanceObject, 0)[0x64 / 4])(
            appearanceObject, nullptr);

    appearance[0] = CSF_AT(std::uint32_t, values, 0);
    appearance[1] = CSF_AT(std::uint32_t, values, 4);
    appearance[2] = CSF_AT(std::uint32_t, values, 8);
    appearance[3] = CSF_AT(volatile std::uint32_t, values, 0xc);
    appearance[3] = 0x42c80000u; // 100.0f

    return reinterpret_cast<SetValuesWithResult>(
        CSF_AT(void**, appearanceObject, 0)[0x60 / 4])(
            appearanceObject, nullptr, appearance);
}

HOOK(0x00529B90, CMenuMessageBox_UpdateHorizontalAnimation, "auto");
