#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAutonomous_SerializeMember14, 0x004D17A0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiAutonomous_SerializeMember18, 0x004D1830,
         (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0,
         (void* self, void* edx, int a2));

static int __fastcall S_0041f0a0_SerializeBaseState(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* archive)
{
    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, S_0041f0a0*))
                       CSF_AT(void**, stream, 0)[0x80 / 4])(
            stream, nullptr, 0, archive);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    Archive_SerializeBaseFields(self, nullptr, reinterpret_cast<int>(archive));

    failed = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                 CSF_AT(void**, stream, 0)[0x44 / 4])(
                stream, nullptr, reinterpret_cast<uint8_t*>(self) + 0x34, archive);
        CSF_AT(uint8_t, archive, 0x00) = result == -1;
    }

    failed = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                 CSF_AT(void**, stream, 0)[0x44 / 4])(
                stream, nullptr, reinterpret_cast<uint8_t*>(self) + 0x35, archive);
        CSF_AT(uint8_t, archive, 0x00) = result == -1;
    }

    failed = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                 CSF_AT(void**, stream, 0)[0x44 / 4])(
                stream, nullptr, reinterpret_cast<uint8_t*>(self) + 0x36, archive);
        CSF_AT(uint8_t, archive, 0x00) = result == -1;
    }

    failed = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAutonomous_SerializeMember14(
            reinterpret_cast<uint8_t*>(self) + 0x14,
            nullptr, reinterpret_cast<int>(archive));
    }

    failed = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAutonomous_SerializeMember18(
            reinterpret_cast<uint8_t*>(self) + 0x18, nullptr, archive);
    }

    failed = CSF_AT(uint8_t, archive, 0x00) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                 CSF_AT(void**, stream, 0)[0x48 / 4])(
                stream, nullptr, reinterpret_cast<uint8_t*>(self) + 0x38, archive);
        CSF_AT(uint8_t, archive, 0x00) = result == -1;
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        const int result =
            ((int(__fastcall*)(void*, void*, S_0041f0a0*))
                 CSF_AT(void**, stream, 0)[0x7C / 4])(
                stream, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x004D19C0, S_0041f0a0_SerializeBaseState, "auto");
