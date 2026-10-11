#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __fastcall C_00869cd0_FindEntryIndexByName(
    C_00869cd0* self, void* /*edx*/, char* name)
{
    auto* entries = CSF_AT(std::uint8_t*, self, 0x04);
    const std::uint32_t beginAddress = CSF_AT(std::uint32_t, entries, 0x04);

    std::uint32_t count = 0;
    if (beginAddress != 0)
    {
        const std::int32_t byteCount = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, entries, 0x08) - beginAddress);
        const std::int64_t product = 0x66666667LL * byteCount;
        const std::int32_t highProduct =
            static_cast<std::int32_t>(product >> 32);
        const std::int32_t scaledCount = highProduct >> 5;
        count = static_cast<std::uint32_t>(
            scaledCount + static_cast<std::int32_t>(
                static_cast<std::uint32_t>(scaledCount) >> 31));
    }

    if (count == 0)
        return -1;

    std::uint32_t offset = 0;
    for (std::uint32_t index = 0; index < count; ++index, offset += 80)
    {
        entries = CSF_AT(std::uint8_t*, self, 0x04);
        auto* entry = CSF_AT(std::uint8_t*, entries, 0x04) + offset;

        const std::uint32_t capacity = CSF_AT(std::uint32_t, entry, 0x1C);
        const char* entryName = capacity < 0x10
            ? reinterpret_cast<const char*>(entry + 0x08)
            : CSF_AT(const char*, entry, 0x08);

        if (entryName && name && __stricmp(entryName, name) == 0)
            return static_cast<int>(index);
    }

    return -1;
}

HOOK(0x00755AA0, C_00869cd0_FindEntryIndexByName, "auto");
