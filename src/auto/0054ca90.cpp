#include <cstdint>
#include "hook.h"
#include "globals.h"

static int __stdcall TypeRegistry_SerializePlane(void* plane, void* context)
{
    auto* planeBytes = static_cast<uint8_t*>(plane);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, const char*, void*))
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, CSF_STR(0x00844378, "CPlane"), context);
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
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr, planeBytes, context) == -1;
    }

    bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr, planeBytes + 12, context) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, planeBytes + 24, context) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, planeBytes + 28, context) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, planeBytes + 32, context) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, planeBytes + 36, context) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        failed = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, serializer, 0)[0x44 / 4])(
                serializer, nullptr, planeBytes + 40, context) == -1;
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

HOOK(0x0054CA90, TypeRegistry_SerializePlane, "auto");
