#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall Vehicle_ProcessRecordFields(void* self, void* /*edx*/, void* context)
{
    bool stopped = false;
    if (CSF_AT(std::uint8_t, context, 0x00) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        stopped = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, processor, 0x00)[0x80 / 4])(
                processor, nullptr, 0, context);
    }

    const std::uint32_t savedState = CSF_AT(std::uint32_t, context, 0x1C);
    CSF_AT(std::uint8_t, context, 0x00) = stopped;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;

    if (!stopped)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0x00)[0x58 / 4])(
                    processor, nullptr, self, context) == -1;
    }

    bool continueProcessing = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0x00)[0x54 / 4])(
                    processor, nullptr, static_cast<std::uint8_t*>(self) + 4, context) == -1;
    }

    continueProcessing = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0x00)[0x54 / 4])(
                    processor, nullptr, static_cast<std::uint8_t*>(self) + 8, context) == -1;
    }

    continueProcessing = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0x00)[0x58 / 4])(
                    processor, nullptr, static_cast<std::uint8_t*>(self) + 12, context) == -1;
    }

    continueProcessing = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0x00)[0x54 / 4])(
                    processor, nullptr, static_cast<std::uint8_t*>(self) + 16, context) == -1;
    }

    continueProcessing = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0x00)[0x54 / 4])(
                    processor, nullptr, static_cast<std::uint8_t*>(self) + 20, context) == -1;
    }

    continueProcessing = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0x00)[0x54 / 4])(
                    processor, nullptr, static_cast<std::uint8_t*>(self) + 24, context) == -1;
    }

    continueProcessing = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, processor, 0x00)[0x54 / 4])(
                    processor, nullptr, static_cast<std::uint8_t*>(self) + 28, context) == -1;
    }

    continueProcessing = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = savedState;
    if (continueProcessing)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        const bool result = ((bool(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, processor, 0x00)[0x7C / 4])(
                processor, nullptr, context);
        if (result)
        {
            CSF_AT(std::uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x0041A950, Vehicle_ProcessRecordFields, "auto");
