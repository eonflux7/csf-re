#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, MpGameModes_GetIntegerParameter, 0x00744240, (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, MpGameModes_GetFloatParameter, 0x007442A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, ForwardFloatToGlobalObject, 0x0073C6B0, (float a1));
CSF_FUNC(char, __stdcall, InterpolateRangeChecked, 0x0073C6C0, (float start, float end, float step));
CSF_FUNC(int, __stdcall, InvokeGlobalFloatMethod, 0x0073C6D0, (std::uint32_t firstBits, std::uint32_t secondBits, std::uint32_t thirdBits));
CSF_FUNC(int, __fastcall, Sub_68F650, 0x0068F650, (void* self, void* edx, float a1));
CSF_GLOBAL(uint8_t, g_NetZFloatSink_8BABC0, 0x008BABC0);

static void* MpGameModes_GetParameterObject(uint32_t owner)
{
    const uintptr_t base = owner ? static_cast<uintptr_t>(owner) - 0xD8 : 0;
    return reinterpret_cast<void*>(base + 0x58);
}

static std::uint32_t MpGameModes_FloatToBits(float value)
{
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof bits);
    return bits;
}

static int __fastcall MpGameModes_ApplyFloatParameterSettings(void* self, void* /*edx*/)
{
    if (MpGameModes_GetIntegerParameter(
            MpGameModes_GetParameterObject(CSF_AT(uint32_t, self, 0x4)), nullptr, 29) == 0)
    {
        GetGlobalDword_73C020();
        return Sub_68F650(reinterpret_cast<void*>(&g_NetZFloatSink_8BABC0), nullptr, 0.08726646f);
    }

    const int mode = MpGameModes_GetIntegerParameter(
        MpGameModes_GetParameterObject(CSF_AT(uint32_t, self, 0x4)), nullptr, 30);

    switch (mode)
    {
    case 0:
    {
        const float value = static_cast<float>(MpGameModes_GetFloatParameter(
            MpGameModes_GetParameterObject(CSF_AT(uint32_t, self, 0x4)), nullptr, 22));
        GetGlobalDword_73C020();
        return ForwardFloatToGlobalObject(value);
    }

    case 1:
    {
        void* parameters = MpGameModes_GetParameterObject(CSF_AT(uint32_t, self, 0x4));
        const float step = static_cast<float>(
            MpGameModes_GetFloatParameter(parameters, nullptr, 25));
        const float end = static_cast<float>(
            MpGameModes_GetFloatParameter(parameters, nullptr, 24));
        const float start = static_cast<float>(
            MpGameModes_GetFloatParameter(parameters, nullptr, 23));
        GetGlobalDword_73C020();
        return InterpolateRangeChecked(start, end, step);
    }

    case 2:
    {
        void* parameters = MpGameModes_GetParameterObject(CSF_AT(uint32_t, self, 0x4));
        const float third = static_cast<float>(
            MpGameModes_GetFloatParameter(parameters, nullptr, 25));
        const float second = static_cast<float>(
            MpGameModes_GetFloatParameter(parameters, nullptr, 24));
        const float first = static_cast<float>(
            MpGameModes_GetFloatParameter(parameters, nullptr, 23));
        GetGlobalDword_73C020();
        return InvokeGlobalFloatMethod(
            MpGameModes_FloatToBits(first),
            MpGameModes_FloatToBits(second),
            MpGameModes_FloatToBits(third));
    }

    default:
        return static_cast<int>(static_cast<uint32_t>(mode) - 2);
    }
}

HOOK(0x0054F120, MpGameModes_ApplyFloatParameterSettings, "auto");
