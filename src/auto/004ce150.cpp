#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0041f0a0_SerializeAutonomousState_4D0860, 0x004D0860, (S_0041f0a0* self, void* , S_0041f0a0* context));

static int __fastcall S_0041f0a0_SerializeExtendedAutonomousState(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* context)
{
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, S_0041f0a0*))
            CSF_AT(void**, serializer, 0)[128 / 4])(
                serializer, nullptr, 0, context);
    }

    uint32_t savedContext = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    S_0041f0a0_SerializeAutonomousState_4D0860(self, nullptr, context);

    bool serializeField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (serializeField)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[88 / 4])(
                    serializer, nullptr, reinterpret_cast<uint8_t*>(self) + 0x6C,
                    context) == -1;
    }

    serializeField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (serializeField)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[88 / 4])(
                    serializer, nullptr, reinterpret_cast<uint8_t*>(self) + 0x70,
                    context) == -1;
    }

    serializeField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (serializeField)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[56 / 4])(
                    serializer, nullptr, reinterpret_cast<uint8_t*>(self) + 0x74,
                    context) == -1;
    }

    serializeField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (serializeField)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[84 / 4])(
                    serializer, nullptr, reinterpret_cast<uint8_t*>(self) + 0x80,
                    context) == -1;
    }

    serializeField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (serializeField)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[84 / 4])(
                    serializer, nullptr, reinterpret_cast<uint8_t*>(self) + 0x84,
                    context) == -1;
    }

    serializeField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (serializeField)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[56 / 4])(
                    serializer, nullptr, reinterpret_cast<uint8_t*>(self) + 0x88,
                    context) == -1;
    }

    serializeField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (serializeField)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[56 / 4])(
                    serializer, nullptr, reinterpret_cast<uint8_t*>(self) + 0x94,
                    context) == -1;
    }

    serializeField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (serializeField)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[8 / 4])(
                    serializer, nullptr, reinterpret_cast<uint8_t*>(self) + 0xA0,
                    context) == -1;
    }

    serializeField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (serializeField)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[68 / 4])(
                    serializer, nullptr, reinterpret_cast<uint8_t*>(self) + 0xE0,
                    context) == -1;
    }

    serializeField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (serializeField)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[68 / 4])(
                    serializer, nullptr, reinterpret_cast<uint8_t*>(self) + 0xE1,
                    context) == -1;
    }

    serializeField = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (serializeField)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[68 / 4])(
                    serializer, nullptr, reinterpret_cast<uint8_t*>(self) + 0xE2,
                    context) == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, context, 0x00) != 0)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    void* serializer = CSF_AT(void*, context, 0x04);
    int result = ((int(__fastcall*)(void*, void*, S_0041f0a0*))
        CSF_AT(void**, serializer, 0)[124 / 4])(serializer, nullptr, context);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0x00) = 0;
    return result;
}

HOOK(0x004CE150, S_0041f0a0_SerializeExtendedAutonomousState, "auto");
