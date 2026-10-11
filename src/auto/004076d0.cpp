#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall Archive_SerializeFloatQuad(
    void* self, void* /*edx*/, void* context)
{
    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* archive = CSF_AT(void*, context, 4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, archive, 0)[0x80 / 4])(
                archive, nullptr, 0, context);
    }

    CSF_AT(uint8_t, context, 0) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* archive = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, archive, 0)[0x54 / 4])(
                    archive, nullptr, self, context) == -1;
    }

    bool canContinue = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* archive = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, archive, 0)[0x54 / 4])(
                    archive, nullptr,
                    static_cast<uint8_t*>(self) + 4, context) == -1;
    }

    canContinue = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* archive = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, archive, 0)[0x54 / 4])(
                    archive, nullptr,
                    static_cast<uint8_t*>(self) + 8, context) == -1;
    }

    canContinue = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* archive = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, archive, 0)[0x54 / 4])(
                    archive, nullptr,
                    static_cast<uint8_t*>(self) + 12, context) == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0) != 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    void* archive = CSF_AT(void*, context, 4);
    // The original tests only AL but returns the full EAX from this slot.
    const int callResult =
        reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, archive, 0)[0x7C / 4])(
                archive, nullptr, context);
    if ((callResult & 0xFF) == 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0) = 0;
    return callResult;
}

HOOK(0x004076D0, Archive_SerializeFloatQuad, "auto");
