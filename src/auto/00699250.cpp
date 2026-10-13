#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using EntryComparator = int (__cdecl*)(const void*, const void*);

CSF_FUNC(int, __cdecl, _tolower, 0x007B1905, (int C));
CSF_FUNC(void*, __cdecl, _bsearch, 0x007B2EBD,
         (const void* key, const void* base, std::size_t count,
          std::size_t entrySize, EntryComparator compare));

static int __fastcall S_00699250_MatchBucketEntry(
    S_00699250* self, void* /*edx*/, unsigned int code,
    std::uint8_t* found)
{
    alignas(8) std::uint8_t keyBytes[0x8];
    keyBytes[0] = static_cast<std::uint8_t>(code >> 24);
    keyBytes[1] = static_cast<std::uint8_t>(code >> 16);
    keyBytes[2] = static_cast<std::uint8_t>(code >> 8);
    keyBytes[3] = static_cast<std::uint8_t>(code);
    keyBytes[4] = 0;

    if (keyBytes[0]) {
        std::uint8_t* character = keyBytes;
        do {
            *character = static_cast<std::uint8_t>(
                _tolower(static_cast<std::int8_t>(*character)));
            ++character;
        } while (*character);
    }

    std::uint32_t normalizedCode = 0;
    if (keyBytes[0]) {
        std::size_t length = 0;
        while (keyBytes[length])
            ++length;

        if (length == 4) {
            // Each original character is sign-extended before being ORed in.
            normalizedCode = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(static_cast<std::int8_t>(keyBytes[0])));
            for (std::size_t i = 1; i < 4; ++i) {
                normalizedCode = (normalizedCode << 8) |
                    static_cast<std::uint32_t>(
                        static_cast<std::int32_t>(
                            static_cast<std::int8_t>(keyBytes[i])));
            }
        }
    }

    *found = 0;
    const std::uint32_t baseAddress = self->field_0008;
    CSF_AT(std::uint32_t, keyBytes, 0x0) = 0;
    CSF_AT(std::uint32_t, keyBytes, 0x4) = normalizedCode;

    const std::size_t count = baseAddress
        ? static_cast<std::size_t>(
              static_cast<std::int32_t>(self->field_000c - baseAddress) >> 3)
        : 0;

    void* match = _bsearch(
        keyBytes, reinterpret_cast<const void*>(baseAddress), count, 8u,
        reinterpret_cast<EntryComparator>(CSF_ADDR_FN(0x00698E20)));
    if (!match)
        return 0;

    const std::int32_t index = static_cast<std::int32_t>(
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(match)) -
        self->field_0008) >> 3;
    if (index == -1)
        return 0;

    *found = 1;
    const std::uint32_t entryAddress =
        self->field_0008 + static_cast<std::uint32_t>(index) * 8u;
    return CSF_AT(int, reinterpret_cast<void*>(entryAddress), 0x0);
}

HOOK(0x00699250, S_00699250_MatchBucketEntry, "auto");
