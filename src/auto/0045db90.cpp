#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, DestinationDispatchWithArgs, 0x0044C440, (void* self, void* edx, int a2, void* a3));

// Reads a float from a nested object, truncates it to an int, takes the
// string payload buffer from a short/long-string object, and forwards both
// to the default destination dispatcher.
static C_0082d410* __stdcall DispatchStringPayloadWithFloatArg(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    // Locate the string object: v3 = *(a3[1] + 4)
    uint32_t* outerNode = reinterpret_cast<uint32_t*>(a3[1]);
    uint8_t* stringObj = reinterpret_cast<uint8_t*>(outerNode[1]);

    // Capacity lives at +0x1C. Below 16 the characters are stored inline at +8,
    // otherwise +8 holds a pointer to the heap buffer.
    uint32_t capacity = CSF_AT(uint32_t, stringObj, 0x1C);
    uint8_t* payloadPtr;
    if (capacity < 0x10)
        payloadPtr = stringObj + 8;
    else
        payloadPtr = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, stringObj, 0x08));

    // Float value: *(float*)(*(*a3 + 4) + 4), truncated to int.
    uint8_t* floatHolder = reinterpret_cast<uint8_t*>(*reinterpret_cast<uint32_t*>(*a3 + 4));
    float floatValue = CSF_AT(float, floatHolder, 0x04);
    int truncatedArg = static_cast<int>(static_cast<int64_t>(floatValue));

    int globalValue = GetGlobalValue_8B0568();
    void* destination = DefaultDestination_Get(reinterpret_cast<void*>(globalValue), nullptr);
    return reinterpret_cast<C_0082d410*>(
        DestinationDispatchWithArgs(destination, nullptr, truncatedArg, payloadPtr));
}

HOOK(0x0045DB90, DispatchStringPayloadWithFloatArg, "auto");
