#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __stdcall TypeRegistry_SerializeSphere(int* sphere, int contextValue)
{
    void* context = reinterpret_cast<void*>(contextValue);

    bool failed = CSF_AT(uint8_t, context, 0x00) == 0 &&
        !((bool(__fastcall*)(void*, void*, const char*, void*))
            CSF_AT(void**, CSF_AT(void*, context, 0x04), 0)[0x80 / 4])(
                CSF_AT(void*, context, 0x04), nullptr,
                CSF_STR(0x00844364, "CSphere"), context);

    const bool writing = CSF_AT(uint8_t, context, 0x14) != 0;
    CSF_AT(uint8_t, context, 0x00) = failed;

    alignas(8) uint8_t centerBytes[0xC];
    int* center = reinterpret_cast<int*>(centerBytes);
    center[0] = 0;
    center[1] = 0;
    center[2] = 0;

    if (writing)
    {
        center[0] = sphere[0];
        const int y = sphere[1];
        const int z = sphere[2];
        center[1] = y;
        const int radius = sphere[3];
        center[2] = z;
        contextValue = radius;
    }

    CSF_AT(int, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, int*, void*))
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr, center, context) == -1;
    }

    const bool serializeRadius = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(int, context, 0x1C) = 0;
    if (serializeRadius)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, int*, void*))
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &contextValue, context) == -1;
    }

    if (CSF_AT(uint8_t, context, 0x14) == 0)
    {
        const int x = center[0];
        const int y = center[1];
        sphere[0] = x;
        const int z = center[2];
        sphere[1] = y;
        const int radius = contextValue;
        sphere[2] = z;
        sphere[3] = radius;
    }

    if (CSF_AT(uint8_t, context, 0x00) != 0 ||
        !((bool(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, CSF_AT(void*, context, 0x04), 0)[0x7C / 4])(
                CSF_AT(void*, context, 0x04), nullptr, context))
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return -1;
    }

    CSF_AT(uint8_t, context, 0x00) = 0;
    return 0;
}

HOOK(0x0054C640, TypeRegistry_SerializeSphere, "auto");
