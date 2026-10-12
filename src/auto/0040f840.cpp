#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall Vehicle_SerializeWheelValue(void* self, void* /*edx*/, void* context)
{
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, context);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, self, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, static_cast<uint8_t*>(self) + 4, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, static_cast<uint8_t*>(self) + 8, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr, static_cast<uint8_t*>(self) + 12, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, static_cast<uint8_t*>(self) + 16, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, static_cast<uint8_t*>(self) + 20, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr, static_cast<uint8_t*>(self) + 13, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const bool result =
            reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, context);
        if (result)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x0040F840, Vehicle_SerializeWheelValue, "auto");
