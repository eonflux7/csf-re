#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __stdcall SerializeVector3(void* vector, void* context)
{
    const std::uintptr_t vectorAddress =
        reinterpret_cast<std::uintptr_t>(vector);

    bool failed = false;
    if (CSF_AT(std::uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed =
            ((std::uint8_t(__fastcall*)(void*, void*, const char*, void*))
                CSF_AT(void**, serializer, 0x00)[32])(
                    serializer, nullptr,
                    CSF_STR(0x00844344, "CVector3"), context) == 0;
    }

    CSF_AT(std::uint8_t, context, 0x00) =
        static_cast<std::uint8_t>(failed);
    CSF_AT(std::uint32_t, context, 0x1C) = 0;

    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;

        const int result =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0x00)[21])(
                    serializer, nullptr, vector, context);

        CSF_AT(std::uint8_t, context, 0x00) =
            static_cast<std::uint8_t>(result == -1);
    }

    const bool processSecond =
        CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;

    if (processSecond)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;

        const int result =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0x00)[21])(
                    serializer, nullptr,
                    reinterpret_cast<void*>(vectorAddress + 4u), context);

        CSF_AT(std::uint8_t, context, 0x00) =
            static_cast<std::uint8_t>(result == -1);
    }

    const bool processThird =
        CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;

    if (processThird)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;

        const int result =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0x00)[21])(
                    serializer, nullptr,
                    reinterpret_cast<void*>(vectorAddress + 8u), context);

        const bool thirdFailed = result == -1;
        CSF_AT(std::uint8_t, context, 0x00) =
            static_cast<std::uint8_t>(thirdFailed);

        if (!thirdFailed)
        {
            serializer = CSF_AT(void*, context, 0x04);
            const std::uint8_t completed =
                ((std::uint8_t(__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, serializer, 0x00)[31])(
                        serializer, nullptr, context);

            if (completed != 0)
            {
                CSF_AT(std::uint8_t, context, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(std::uint8_t, context, 0x00) = 1;
    return -1;
}

HOOK(0x0054C410, SerializeVector3, "auto");
