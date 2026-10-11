#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAutonomous_SerializeBaseState, 0x004D19C0, (void* self, void* edx, void* a2));

static int __fastcall S_0041f0a0_SerializeAutonomousStateSwappedOffsets(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* state)
{
    bool initialFailure = false;
    if (CSF_AT(uint8_t, state, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        initialFailure =
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int, S_0041f0a0*)>(
                CSF_AT(void**, serializer, 0)[0x80 / 4])(
                    serializer, nullptr, 0, state);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, state, 0x1C);
    CSF_AT(uint8_t, state, 0x00) = initialFailure;
    AiAutonomous_SerializeBaseState(self, nullptr, state);

    bool canSerialize = CSF_AT(uint8_t, state, 0x00) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, S_0041f0a0*)>(
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, &self->field_0040, state);
        CSF_AT(uint8_t, state, 0x00) = result == -1;
    }

    canSerialize = CSF_AT(uint8_t, state, 0x00) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x3C, state);
        CSF_AT(uint8_t, state, 0x00) = result == -1;
    }

    CSF_AT(uint32_t, state, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, state, 0x00) != 0)
    {
        CSF_AT(uint8_t, state, 0x00) = 1;
        return 1;
    }

    void* serializer = CSF_AT(void*, state, 0x04);
    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, S_0041f0a0*)>(
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, state);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, state, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, state, 0x00) = 0;
    return result;
}

HOOK(0x004D0D40, S_0041f0a0_SerializeAutonomousStateSwappedOffsets, "auto");
