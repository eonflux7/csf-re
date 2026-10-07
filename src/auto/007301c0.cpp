#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static float CGuiSwitch_ReadBlinkDuration(const CGuiSwitch* self)
{
    float duration;
    std::memcpy(&duration, &self->field_0118, sizeof(duration));
    return duration;
}

static void __fastcall CGuiSwitch_UpdateBlink(
    CGuiSwitch* self, void* /*edx*/, float deltaTime)
{
    if (self->m_pText == 0)
        return;

    if (CGuiSwitch_ReadBlinkDuration(self) == g_fZero)
        return;

    const auto isActive =
        reinterpret_cast<std::uint8_t(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xBC / 4]);

    if (!isActive(self, nullptr))
    {
        // The original copies the float's bits without an FP load/store.
        std::memcpy(&self->field_0150, &self->field_0158, sizeof(float));
        return;
    }

    double elapsed =
        static_cast<double>(deltaTime) + static_cast<double>(self->field_014c);
    const bool reset =
        elapsed >= static_cast<double>(CGuiSwitch_ReadBlinkDuration(self));

    self->field_014c = static_cast<float>(elapsed);
    if (reset)
        elapsed = static_cast<double>(g_fZero);
    self->field_014c = static_cast<float>(elapsed);

    const double halfDuration =
        static_cast<double>(CGuiSwitch_ReadBlinkDuration(self)) *
        static_cast<double>(g_HalfDurationScale);
    const double distance = elapsed < halfDuration
        ? halfDuration - elapsed
        : elapsed - halfDuration;
    const double fraction = distance / halfDuration;

    const double range =
        static_cast<double>(self->field_0158) -
        static_cast<double>(self->field_0154);
    self->field_0150 = static_cast<float>(
        range * fraction + static_cast<double>(self->field_0154));
}

HOOK(0x007301C0, CGuiSwitch_UpdateBlink, "auto");
