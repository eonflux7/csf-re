#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_44D190, 0x0044D190, (void* self, void* edx, int arg0, int a2, int* a4));

static int __stdcall ForwardToDefaultDestinationIfEnabled(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    // a3[2] -> record whose +4 points to a block holding a flag byte at +1
    uint32_t* flagHolder = reinterpret_cast<uint32_t*>(a3[2]);
    uint8_t* flagBlock = reinterpret_cast<uint8_t*>(flagHolder[1]);
    uint8_t flagByte = flagBlock[1];

    // Local 4-byte slot that receives the flag byte; its address is passed to sub_44D190.
    int flagStorage = 0;
    uint8_t* flagByteSlot = reinterpret_cast<uint8_t*>(&flagStorage);
    *flagByteSlot = 0;
    *flagByteSlot = flagByte;

    // a3[3] -> block whose +4 points to a float at +4 (scale value, truncated to int in the original)
    uint32_t* scaleHolder = reinterpret_cast<uint32_t*>(a3[3]);
    uint8_t* scaleBlock = reinterpret_cast<uint8_t*>(scaleHolder[1]);
    float scaleFloat = *reinterpret_cast<float*>(scaleBlock + 4);
    [[maybe_unused]] int scaleScratch = static_cast<int>(scaleFloat);

    // a3[1] -> record; its +4 is the result value (also the returned value when disabled)
    uint32_t* linkHolder = reinterpret_cast<uint32_t*>(a3[1]);
    uint32_t resultValue = linkHolder[1];

    // Field at +0x0C of the result record: forwarding only when it is non-negative (signed compare)
    int32_t enabledField = CSF_AT(int32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(resultValue)), 0x0C);
    if (enabledField >= 0)
    {
        // a3[0] -> block whose +4 points to a float at +4 (distance, truncated to int)
        uint32_t* distanceHolder = reinterpret_cast<uint32_t*>(a3[0]);
        uint8_t* distanceBlock = reinterpret_cast<uint8_t*>(distanceHolder[1]);
        float distanceFloat = *reinterpret_cast<float*>(distanceBlock + 4);
        int distanceInt = static_cast<int>(distanceFloat);

        int resultPlusFour = static_cast<int>(resultValue + 4);

        int globalValue = GetGlobalValue_8B0568();
        void* defaultDestination = DefaultDestination_Get(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);

        return sub_44D190(defaultDestination, nullptr, distanceInt, resultPlusFour, &flagStorage);
    }

    return static_cast<int>(resultValue);
}

HOOK(0x0045B390, ForwardToDefaultDestinationIfEnabled, "auto");
