#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __stdcall TypeRegistry_SerializeRect(void* rect, void* context)
{
    auto* rectBytes = static_cast<uint8_t*>(rect);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, const char*, void*))
                       CSF_AT(void**, serializer, 0)[0x80 / 4])(
            serializer, nullptr, CSF_STR(0x00844390, "SRect"), context);
    }

    CSF_AT(uint8_t, context, 0x00) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, serializer, 0)[0x58 / 4])(
                serializer, nullptr, rectBytes, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, serializer, 0)[0x58 / 4])(
                serializer, nullptr, rectBytes + 8, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, serializer, 0)[0x58 / 4])(
                serializer, nullptr, rectBytes + 4, context) == -1;
    }

    failed = CSF_AT(uint8_t, context, 0x00) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        failed =
            ((int(__fastcall*)(void*, void*, void*, void*))
                 CSF_AT(void**, serializer, 0)[0x58 / 4])(
                serializer, nullptr, rectBytes + 12, context) == -1;
        CSF_AT(uint8_t, context, 0x00) = failed;

        if (!failed)
        {
            serializer = CSF_AT(void*, context, 0x04);
            if (((bool(__fastcall*)(void*, void*, void*))
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

HOOK(0x0054CDF0, TypeRegistry_SerializeRect, "auto");
