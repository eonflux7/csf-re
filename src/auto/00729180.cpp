#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60,
         (float* value, float* minimum, float* maximum));

static uint8_t __fastcall C_00865870_UpdateTransition(
    C_00865870* self, void* /*edx*/, float deltaTime)
{
    struct Value {
        uint32_t components[3];
        float amount;
    };

    void* transition = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        self->vtbl[0x18 / 4])(self, nullptr);

    const uint8_t active = self->field_0048;
    if (!active || !transition)
        return active;

    void* destination = reinterpret_cast<void*>(self->field_0030);
    if (!destination)
        return active;

    void* current = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, destination, 0)[0x30 / 4])(destination, nullptr);

    const float elapsed = self->field_0044;
    Value value;
    value.components[0] = CSF_AT(uint32_t, current, 0x00);
    value.components[1] = CSF_AT(uint32_t, current, 0x04);
    value.components[2] = CSF_AT(uint32_t, current, 0x08);
    std::memcpy(&value.amount, static_cast<uint8_t*>(current) + 0x0C, 4);

    // The x87 C0 test also takes this branch for unordered comparisons.
    const bool progressing =
        !(elapsed >= CSF_AT(float, transition, 0x120));

    Value target = {{0, 0, 0}, 1.0f};
    auto getTarget = reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
        self->vtbl[0x64 / 4]);

    if (progressing) {
        // Division uses the unrounded sum still held in the x87 register.
        const double updatedElapsed =
            static_cast<double>(deltaTime) + self->field_0044;
        self->field_0044 = static_cast<float>(updatedElapsed);
        deltaTime = static_cast<float>(
            updatedElapsed / CSF_AT(float, transition, 0x120));

        getTarget(self, nullptr, &target);

        const uint8_t forward =
            reinterpret_cast<uint8_t (__fastcall*)(void*, void*)>(
                CSF_AT(void**, transition, 0)[0xBC / 4])(
                    transition, nullptr);

        double amount = static_cast<double>(target.amount) * deltaTime;
        if (!forward)
            amount = static_cast<double>(target.amount) - amount;
        value.amount = static_cast<float>(amount);

        deltaTime = 0.0f;
        ClampFloatToBounds(&value.amount, &deltaTime, &target.amount);

        destination = reinterpret_cast<void*>(self->field_0030);
    } else {
        void* completed = getTarget(self, nullptr, &target);
        std::memcpy(&value.amount,
                    static_cast<uint8_t*>(completed) + 0x0C, 4);
        destination = reinterpret_cast<void*>(self->field_0030);
        self->field_0048 = 0;
    }

    return static_cast<uint8_t>(
        reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, destination, 0)[0x2C / 4])(
                destination, nullptr, &value));
}

HOOK(0x00729180, C_00865870_UpdateTransition, "auto");
