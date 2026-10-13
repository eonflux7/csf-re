#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0041f0a0_SerializeAutonomousState_4D0860, 0x004D0860, (S_0041f0a0* self, void* , S_0041f0a0* context));

static int __fastcall S_0041f0a0_SerializeExtendedState(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* context)
{
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, S_0041f0a0*))
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, context);
    }

    uint32_t savedContext = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    S_0041f0a0_SerializeAutonomousState_4D0860(self, nullptr, context);

    bool canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x6C, context) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x70, context) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x74, context) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x78, context) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x7C, context) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[0x38 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x88, context) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[0x08 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x98, context) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x94, context) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x95, context) == -1;
    }

    canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, serializer, 0)[0x44 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x96, context) == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        // The original tests AL but forwards the entire EAX on success.
        int result = ((int(__fastcall*)(void*, void*, S_0041f0a0*))
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x004A9550, S_0041f0a0_SerializeExtendedState, "auto");
