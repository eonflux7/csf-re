#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall SerializeFourMembers(void* self, void* /*edx*/, void* context)
{
    bool finished = CSF_AT(std::uint8_t, context, 0x00) == 0;
    if (finished)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        finished = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, context);
    }

    const std::uint32_t savedState = CSF_AT(std::uint32_t, context, 0x1C);
    CSF_AT(std::uint8_t, context, 0x00) = finished;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;

    if (!finished)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr, self, context) == -1;
    }

    finished = CSF_AT(std::uint8_t, context, 0x00) != 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (!finished)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr,
                    static_cast<std::uint8_t*>(self) + 12, context) == -1;
    }

    finished = CSF_AT(std::uint8_t, context, 0x00) != 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (!finished)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr,
                    static_cast<std::uint8_t*>(self) + 24, context) == -1;
    }

    finished = CSF_AT(std::uint8_t, context, 0x00) != 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (!finished)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr,
                    static_cast<std::uint8_t*>(self) + 36, context) == -1;
    }

    finished = CSF_AT(std::uint8_t, context, 0x00) != 0;
    CSF_AT(std::uint32_t, context, 0x1C) = savedState;
    if (!finished)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        // Original 0045383a..00453847: test al,al selects the path, but the
        // success path returns the full eax of the callee (ret 4), not a bool.
        const std::uint32_t result =
            reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, context);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x00453770, SerializeFourMembers, "auto");
