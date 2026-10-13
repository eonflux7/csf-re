#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

static int __fastcall S_004382f0_AdvanceAndNotify(
    S_004382f0* self, void* /*edx*/, float delta)
{
    float accumulated = delta + self->field_0010;
    void* notifyTarget = reinterpret_cast<void*>(self->field_0004);
    bool belowOrUnordered = !(accumulated >= self->field_0008);
    self->field_0010 = accumulated;

    uint8_t* notifyArg = reinterpret_cast<uint8_t*>(self) + 0x18;

    if (belowOrUnordered)
    {
        float scaledValue = accumulated / self->field_0008 * g_ColorChannelScale;
        std::memcpy(&self->field_0024, &scaledValue, sizeof(scaledValue));

        return reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, notifyTarget, 0)[0x60 / 4])(
                notifyTarget, nullptr, notifyArg);
    }

    self->field_0024 = 1132396544;
    int result = reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*)>(
        CSF_AT(void**, notifyTarget, 0)[0x60 / 4])(
            notifyTarget, nullptr, notifyArg);
    self->field_0000 = 1;
    self->field_0010 = 0.0f;
    return result;
}

HOOK(0x004382F0, S_004382f0_AdvanceAndNotify, "auto");
