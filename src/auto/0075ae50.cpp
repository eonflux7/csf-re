#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, S_0046bea0_AssignRecords, 0x006D9890, (S_0046bea0* self, void* , int sourceAddress));

static int __cdecl SoundDefs_CopyRecordsBackward(
    int sourceBeginAddress,
    int sourceEndAddress,
    int destinationEndAddress,
    int /*unused*/)
{
    auto* sourceBegin = reinterpret_cast<std::uint8_t*>(sourceBeginAddress);
    auto* source = reinterpret_cast<std::uint8_t*>(sourceEndAddress);
    auto* destination = reinterpret_cast<std::uint8_t*>(destinationEndAddress);

    while (source != sourceBegin)
    {
        source -= 68;
        destination -= 68;

        CSF_AT(std::uint32_t, destination, 0x00) =
            CSF_AT(std::uint32_t, source, 0x00);

        S_0046bea0_AssignRecords(
            reinterpret_cast<S_0046bea0*>(destination + 4),
            nullptr,
            reinterpret_cast<int>(source + 4));

        CSF_AT(std::uint32_t, destination, 0x14) =
            CSF_AT(std::uint32_t, source, 0x14);
        CSF_AT(std::uint32_t, destination, 0x18) =
            CSF_AT(std::uint32_t, source, 0x18);
        CSF_AT(std::uint32_t, destination, 0x1C) =
            CSF_AT(std::uint32_t, source, 0x1C);

        CSF_AT(std::uint8_t, destination, 0x20) =
            CSF_AT(std::uint8_t, source, 0x20);
        CSF_AT(std::uint8_t, destination, 0x21) =
            CSF_AT(std::uint8_t, source, 0x21);
        CSF_AT(std::uint8_t, destination, 0x22) =
            CSF_AT(std::uint8_t, source, 0x22);
        CSF_AT(std::uint8_t, destination, 0x23) =
            CSF_AT(std::uint8_t, source, 0x23);
        CSF_AT(std::uint8_t, destination, 0x24) =
            CSF_AT(std::uint8_t, source, 0x24);
        CSF_AT(std::uint8_t, destination, 0x25) =
            CSF_AT(std::uint8_t, source, 0x25);
        CSF_AT(std::uint8_t, destination, 0x26) =
            CSF_AT(std::uint8_t, source, 0x26);
        CSF_AT(std::uint8_t, destination, 0x27) =
            CSF_AT(std::uint8_t, source, 0x27);

        AssignStringSubstring(destination + 40, nullptr, source + 40, 0, -1);
    }

    return reinterpret_cast<int>(destination);
}

HOOK(0x0075AE50, SoundDefs_CopyRecordsBackward, "auto");
