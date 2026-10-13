#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, S_0046bea0_AssignRecords, 0x006D9890, (S_0046bea0* self, void* , int sourceAddress));

static std::uint32_t* __cdecl SoundDefs_FillRecordRange(
    std::uint32_t* first, std::uint32_t* last, int sourceAddress)
{
    const auto* source = reinterpret_cast<const std::uint8_t*>(sourceAddress);

    for (auto* record = first; record != last; record += 17)
    {
        CSF_AT(std::uint32_t, record, 0x00) =
            CSF_AT(std::uint32_t, source, 0x00);

        S_0046bea0_AssignRecords(
            reinterpret_cast<S_0046bea0*>(
                reinterpret_cast<std::uint8_t*>(record) + 0x04),
            nullptr, sourceAddress + 4);

        CSF_AT(std::uint32_t, record, 0x14) =
            CSF_AT(std::uint32_t, source, 0x14);
        CSF_AT(std::uint32_t, record, 0x18) =
            CSF_AT(std::uint32_t, source, 0x18);
        CSF_AT(std::uint32_t, record, 0x1C) =
            CSF_AT(std::uint32_t, source, 0x1C);
        CSF_AT(std::uint8_t, record, 0x20) =
            CSF_AT(std::uint8_t, source, 0x20);
        CSF_AT(std::uint8_t, record, 0x21) =
            CSF_AT(std::uint8_t, source, 0x21);
        CSF_AT(std::uint8_t, record, 0x22) =
            CSF_AT(std::uint8_t, source, 0x22);
        CSF_AT(std::uint8_t, record, 0x23) =
            CSF_AT(std::uint8_t, source, 0x23);
        CSF_AT(std::uint8_t, record, 0x24) =
            CSF_AT(std::uint8_t, source, 0x24);
        CSF_AT(std::uint8_t, record, 0x25) =
            CSF_AT(std::uint8_t, source, 0x25);
        CSF_AT(std::uint8_t, record, 0x26) =
            CSF_AT(std::uint8_t, source, 0x26);
        CSF_AT(std::uint8_t, record, 0x27) =
            CSF_AT(std::uint8_t, source, 0x27);

        AssignStringSubstring(
            reinterpret_cast<std::uint8_t*>(record) + 0x28,
            nullptr, source + 0x28, 0, -1);
    }

    return last;
}

HOOK(0x0075AFD0, SoundDefs_FillRecordRange, "auto");
