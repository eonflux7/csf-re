#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Archive_SerializeFloatQuad, 0x004076D0, (void* self, void* , void* context));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(int, __fastcall, AiAutonomous_SerializeBaseState, 0x004D19C0, (void* self, void* edx, void* a2));

static int __fastcall S_0041f0a0_SerializeAutonomousState(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* context)
{
    bool failed = false;
    if (CSF_AT(std::uint8_t, context, 0) == 0)
    {
        void* backend = CSF_AT(void*, context, 4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, S_0041f0a0*)>(
            CSF_AT(void**, backend, 0)[0x80 / 4])(backend, nullptr, 0, context);
    }

    const std::uint32_t savedState = CSF_AT(std::uint32_t, context, 0x1C);
    CSF_AT(std::uint8_t, context, 0) = failed;
    AiAutonomous_SerializeBaseState(self, nullptr, context);

    const bool serializeString = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (serializeString)
    {
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x3C),
            reinterpret_cast<S_00691860*>(context));
    }

    const bool serializeQuad = CSF_AT(std::uint8_t, context, 0) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (serializeQuad)
    {
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        Archive_SerializeFloatQuad(
            reinterpret_cast<std::uint8_t*>(self) + 0x5C, nullptr, context);
    }

    CSF_AT(std::uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(std::uint8_t, context, 0) == 0)
    {
        void* backend = CSF_AT(void*, context, 4);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, S_0041f0a0*)>(
                CSF_AT(void**, backend, 0)[0x7C / 4])(backend, nullptr, context);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004D0860, S_0041f0a0_SerializeAutonomousState, "auto");
