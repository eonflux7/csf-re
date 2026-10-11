#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __fastcall C_0086f128_FindCharacterIndex(
    C_0086f128* self, void* /*edx*/, char* name)
{
    auto* entries = CSF_AT(std::uint8_t*, self, 0x08);
    const std::uint32_t beginAddress = CSF_AT(std::uint32_t, entries, 0x04);

    std::uint32_t count = 0;
    if (beginAddress != 0)
    {
        const std::int32_t byteCount = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, entries, 0x08) - beginAddress);
        const std::int64_t product = 0x75DED953LL * byteCount;
        const std::int32_t highProduct =
            static_cast<std::int32_t>(product >> 32);
        const std::int32_t scaledCount = highProduct >> 8;
        count = static_cast<std::uint32_t>(scaledCount);
        count += count >> 31;
    }

    std::uint32_t offset = 0;
    for (std::uint32_t index = 0; index < count; ++index, offset += 556)
    {
        entries = CSF_AT(std::uint8_t*, self, 0x08);
        auto* entry = CSF_AT(std::uint8_t*, entries, 0x04) + offset;

        const std::uint32_t capacity = CSF_AT(std::uint32_t, entry, 0x38);
        const char* entryName = capacity < 0x10
            ? reinterpret_cast<const char*>(entry + 0x24)
            : CSF_AT(const char*, entry, 0x24);

        if (entryName && name && __stricmp(entryName, name) == 0)
            return static_cast<int>(index);
    }

    return -1;
}

HOOK(0x007624F0, C_0086f128_FindCharacterIndex, "auto");
