#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082d478_SerializeBaseState, 0x00474BD0, (void* self, void* edx, void* a2));

static int __fastcall C_00831618_SerializeState(
    C_00831618* self, void* /*edx*/, void* state)
{
    bool initialFailure = false;
    if (CSF_AT(uint8_t, state, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        initialFailure =
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
                CSF_AT(void**, serializer, 0)[0x80 / 4])(
                    serializer, nullptr, 0, state);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, state, 0x1C);
    CSF_AT(uint8_t, state, 0x00) = initialFailure;
    C_0082d478_SerializeBaseState(self, nullptr, state);

    bool canSerialize = CSF_AT(uint8_t, state, 0x00) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, &self->field_0034, state);
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
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr, &self->field_0038, state);
        CSF_AT(uint8_t, state, 0x00) = result == -1;
    }

    CSF_AT(uint32_t, state, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, state, 0x00) != 0)
    {
        CSF_AT(uint8_t, state, 0x00) = 1;
        return 1;
    }

    void* serializer = CSF_AT(void*, state, 0x04);
    // The original tests only al and returns eax as the callee left it.
    const int rawResult =
        reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, state);
    if (static_cast<uint8_t>(rawResult) == 0)
    {
        CSF_AT(uint8_t, state, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, state, 0x00) = 0;
    return rawResult;
}

HOOK(0x00474F10, C_00831618_SerializeState, "auto");
