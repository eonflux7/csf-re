#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall GfxClumpLoader_SerializeFields(
    void* self, void* /*edx*/, void* context)
{
    auto* fields = static_cast<uint8_t*>(self);

    const bool beginFailed =
        CSF_AT(uint8_t, context, 0) == 0 &&
        !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, CSF_AT(void*, context, 4), 0)[128 / 4])(
                CSF_AT(void*, context, 4), nullptr, 0, context);

    const uint32_t savedContext = CSF_AT(uint32_t, context, 28);
    CSF_AT(uint8_t, context, 0) = beginFailed;
    CSF_AT(uint32_t, context, 28) = 0;

    if (!beginFailed)
    {
        void* stream = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 21) = 0;
        CSF_AT(uint8_t, context, 22) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[68 / 4])(
                    stream, nullptr, fields + 57, context) == -1;
    }

    bool failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 28) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 21) = 0;
        CSF_AT(uint8_t, context, 22) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[12 / 4])(
                    stream, nullptr, fields + 60, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 28) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 21) = 0;
        CSF_AT(uint8_t, context, 22) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[12 / 4])(
                    stream, nullptr, fields + 108, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 28) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 21) = 0;
        CSF_AT(uint8_t, context, 22) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[12 / 4])(
                    stream, nullptr, fields + 76, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 28) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 21) = 0;
        CSF_AT(uint8_t, context, 22) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[12 / 4])(
                    stream, nullptr, fields + 92, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 28) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 21) = 0;
        CSF_AT(uint8_t, context, 22) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[68 / 4])(
                    stream, nullptr, fields + 124, context) == -1;
    }

    CSF_AT(uint32_t, context, 28) = savedContext;
    if (CSF_AT(uint8_t, context, 0) != 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    void* stream = CSF_AT(void*, context, 4);
    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, stream, 0)[124 / 4])(
                stream, nullptr, context);

    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0) = 0;
    return result;
}

HOOK(0x006FD690, GfxClumpLoader_SerializeFields, "auto");
