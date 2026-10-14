#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall AiAutonomous_SerializeByteAndPair(
    void* self, void* /*edx*/, void* context)
{
    bool failed = false;
    if (CSF_AT(std::uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 4);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, context);
    }

    const std::uint32_t savedState = CSF_AT(std::uint32_t, context, 0x1C);
    CSF_AT(std::uint8_t, context, 0) = failed;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;

    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr, self, context) == -1;
    }

    bool canContinue = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, context, 4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr,
                    static_cast<std::uint8_t*>(self) + 4, context) == -1;
    }

    canContinue = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, context, 4);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr,
                    static_cast<std::uint8_t*>(self) + 8, context) == -1;
    }

    canContinue = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = savedState;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, context, 4);
        const int result =
            ((int(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, context);
        if ((result & 0xFF) != 0)
        {
            CSF_AT(std::uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004D18F0, AiAutonomous_SerializeByteAndPair, "auto");
