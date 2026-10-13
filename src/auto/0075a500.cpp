#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __fastcall C_0086c1e0_FindSoundIndexByName(
    C_0086c1e0* self, void* /*edx*/, char* String2)
{
    auto* table = reinterpret_cast<std::uint8_t*>(self->field_0004);
    const std::uint32_t beginAddress = CSF_AT(std::uint32_t, table, 0x04);

    std::uint32_t count = 0;
    if (beginAddress != 0)
    {
        const std::int32_t byteCount = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, table, 0x08) - beginAddress);
        const std::int64_t product = 0x78787879LL * byteCount;
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
    for (std::uint32_t index = 0; index < count; ++index, offset += 68)
    {
        table = reinterpret_cast<std::uint8_t*>(self->field_0004);
        auto* entry = CSF_AT(std::uint8_t*, table, 0x04) + offset;

        const std::uint32_t capacity = CSF_AT(std::uint32_t, entry, 0x40);
        const char* entryName = capacity < 0x10
            ? reinterpret_cast<const char*>(entry + 0x2C)
            : CSF_AT(const char*, entry, 0x2C);

        if (entryName && String2 && __stricmp(entryName, String2) == 0)
            return static_cast<int>(index);
    }

    return -1;
}

HOOK(0x0075A500, C_0086c1e0_FindSoundIndexByName, "auto");
