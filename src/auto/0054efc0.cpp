#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, MpGameModes_ProcessFloatRange, 0x0068E3F0, (float a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, ForwardFloatToGlobalHandler, 0x0073C660, (void* , void* , float a1));
CSF_FUNC(char, __stdcall, ProcessRangedFloat, 0x0073C670, (float from, float to, float step));
CSF_FUNC(int, __fastcall, ForwardFloatTripletToGlobalObject, 0x0073C680, (void* , void* edx, float first, float second, float third));
CSF_FUNC(int, __fastcall, MpGameModes_GetIntegerParameter, 0x00744240, (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, MpGameModes_GetFloatParameter, 0x007442A0, (void* self, void* edx, int a2));

static void* MpGameModes_GetParameterObject(void* self)
{
    const uint32_t owner = CSF_AT(uint32_t, self, 0x4);
    const uintptr_t base = owner ? static_cast<uintptr_t>(owner) - 0xD8 : 0;
    return reinterpret_cast<void*>(base + 0x58);
}

static int __fastcall MpGameModes_ApplyFloatRangeSettings(void* self, void* /*edx*/)
{
    if (!MpGameModes_GetIntegerParameter(
            MpGameModes_GetParameterObject(self), nullptr, 20))
    {
        GetGlobalDword_73C020();
        return MpGameModes_ProcessFloatRange(180.0f, 1176256512);
    }

    const int mode = MpGameModes_GetIntegerParameter(
        MpGameModes_GetParameterObject(self), nullptr, 21);

    switch (mode)
    {
    case 0:
    {
        const float value = static_cast<float>(MpGameModes_GetFloatParameter(
            MpGameModes_GetParameterObject(self), nullptr, 13));
        void* handler = reinterpret_cast<void*>(GetGlobalDword_73C020());
        return ForwardFloatToGlobalHandler(handler, nullptr, value);
    }

    case 1:
    case 2:
    {
        void* parameters = MpGameModes_GetParameterObject(self);
        const float third = static_cast<float>(
            MpGameModes_GetFloatParameter(parameters, nullptr, 16));
        const float second = static_cast<float>(
            MpGameModes_GetFloatParameter(parameters, nullptr, 15));
        const float first = static_cast<float>(
            MpGameModes_GetFloatParameter(parameters, nullptr, 14));

        void* handler = reinterpret_cast<void*>(GetGlobalDword_73C020());
        if (mode == 1)
            return ProcessRangedFloat(first, second, third);

        return ForwardFloatTripletToGlobalObject(
            handler, nullptr, first, second, third);
    }

    default:
        return static_cast<int>(static_cast<uint32_t>(mode) - 2);
    }
}

HOOK(0x0054EFC0, MpGameModes_ApplyFloatRangeSettings, "auto");
