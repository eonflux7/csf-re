#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(std::int32_t, g_FallbackContext, 0x008A669C);

static int __fastcall C_0082d448_ProcessRequest(
    C_0082d448* self, void* /*edx*/, int requestAddress)
{
    void* request = reinterpret_cast<void*>(requestAddress);

    std::uint8_t skipProcessing = 0;
    if (CSF_AT(std::uint8_t, request, 0x00) == 0)
    {
        void* object = CSF_AT(void*, request, 0x04);
        const std::uint8_t result =
            ((std::uint8_t(__fastcall*)(void*, void*, int, void*))
                CSF_AT(void**, object, 0x00)[32])(
                    object, nullptr, 0, request);
        if (result == 0)
            skipProcessing = 1;
    }

    const std::uint32_t savedState =
        CSF_AT(std::uint32_t, request, 0x1C);
    CSF_AT(std::uint8_t, request, 0x00) = skipProcessing;
    CSF_AT(std::uint32_t, request, 0x1C) = 0;

    if (skipProcessing == 0)
    {
        void* object = CSF_AT(void*, request, 0x04);
        CSF_AT(std::uint8_t, request, 0x15) = 0;
        CSF_AT(std::uint8_t, request, 0x16) = 0;

        const int result =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, object, 0x00)[22])(
                    object, nullptr, &self->field_0004, request);
        CSF_AT(std::uint8_t, request, 0x00) = (result == -1);
    }

    const bool trySecondContext =
        CSF_AT(std::uint8_t, request, 0x00) == 0;
    CSF_AT(std::uint32_t, request, 0x1C) = 0;

    if (trySecondContext)
    {
        void* object = CSF_AT(void*, request, 0x04);
        CSF_AT(std::uint8_t, request, 0x15) = 0;
        CSF_AT(std::uint8_t, request, 0x16) = 0;

        const int result =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, object, 0x00)[22])(
                    object, nullptr, &self->field_0008, request);
        CSF_AT(std::uint8_t, request, 0x00) = (result == -1);
    }

    const bool tryFallbackContext =
        CSF_AT(std::uint8_t, request, 0x00) == 0;
    CSF_AT(std::uint32_t, request, 0x1C) = 0;

    if (tryFallbackContext)
    {
        void* object = CSF_AT(void*, request, 0x04);
        CSF_AT(std::uint8_t, request, 0x15) = 0;
        CSF_AT(std::uint8_t, request, 0x16) = 0;

        const int result =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, object, 0x00)[22])(
                    object, nullptr, &g_FallbackContext, request);
        CSF_AT(std::uint8_t, request, 0x00) = (result == -1);
    }

    const bool tryFinalOperation =
        CSF_AT(std::uint8_t, request, 0x00) == 0;
    CSF_AT(std::uint32_t, request, 0x1C) = savedState;

    if (tryFinalOperation)
    {
        void* object = CSF_AT(void*, request, 0x04);
        const int result =
            ((int(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, object, 0x00)[31])(
                    object, nullptr, request);

        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, request, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, request, 0x00) = 1;
    return 1;
}

HOOK(0x00691300, C_0082d448_ProcessRequest, "auto");
