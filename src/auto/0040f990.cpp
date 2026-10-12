#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall Vehicle_SerializeEmbeddedState(
    void* self, void* /*edx*/, void* context)
{
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, context);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, serializer, 0)[0x54 / 4])(
                serializer, nullptr, static_cast<uint8_t*>(self) + 8, context);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, serializer, 0)[0x54 / 4])(
                serializer, nullptr, static_cast<uint8_t*>(self) + 12, context);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    failed = CSF_AT(uint8_t, context, 0) != 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, serializer, 0)[0x34 / 4])(
                serializer, nullptr, self, context);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        // The original tests AL but forwards the entire EAX on success.
        const int result = ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x0040F990, Vehicle_SerializeEmbeddedState, "auto");
