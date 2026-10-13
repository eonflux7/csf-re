#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __stdcall TypeRegistry_SerializeCDirAng(void* position, void* context)
{
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, const char*, void*)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, CSF_STR(0x0084435C, "CDirAng"), context);
    }

    CSF_AT(uint8_t, context, 0x00) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, position, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        failed = reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
            CSF_AT(void**, serializer, 0)[0x54 / 4])(
                serializer, nullptr, static_cast<uint8_t*>(position) + 4, context) == -1;
        CSF_AT(uint8_t, context, 0x00) = failed;

        if (!failed)
        {
            serializer = CSF_AT(void*, context, 0x04);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, serializer, 0)[0x7C / 4])(
                        serializer, nullptr, context))
            {
                CSF_AT(uint8_t, context, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return -1;
}

HOOK(0x0054C590, TypeRegistry_SerializeCDirAng, "auto");
