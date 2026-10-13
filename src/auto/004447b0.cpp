#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, ProcessRecordArray_4443B0, 0x004443B0, (std::uint32_t* array, int contextAddress));
CSF_FUNC(uint32_t, __stdcall, ProcessRecordArray_4445B0, 0x004445B0, (void* array, char* context));

static int __fastcall C_0083109c_ProcessEvent(
    C_0083109c* self, void* /*edx*/, int contextAddress)
{
    auto* context = reinterpret_cast<char*>(contextAddress);

    bool stopped = false;
    if (CSF_AT(std::uint8_t, context, 0) == 0)
    {
        void* receiver = CSF_AT(void*, context, 0x04);
        stopped = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, receiver, 0)[0x80 / 4])(
                receiver, nullptr, 0, contextAddress);
    }

    const std::uint32_t savedContext = CSF_AT(std::uint32_t, context, 0x1C);
    CSF_AT(std::uint8_t, context, 0) = stopped;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;

    if (!stopped)
    {
        void* receiver = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;

        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, std::uint32_t*, int)>(
                CSF_AT(void**, receiver, 0)[0x54 / 4])(
                    receiver, nullptr, &self->field_0008, contextAddress);
        CSF_AT(std::uint8_t, context, 0) = result == -1;
    }

    bool continueProcessing = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        ProcessRecordArray_4445B0(&self->field_000c, context);
    }

    continueProcessing = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (continueProcessing)
    {
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        ProcessRecordArray_4443B0(&self->field_001c, contextAddress);
    }

    continueProcessing = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = savedContext;
    if (continueProcessing)
    {
        void* receiver = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, receiver, 0)[0x7C / 4])(
                    receiver, nullptr, contextAddress);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004447B0, C_0083109c_ProcessEvent, "auto");
