#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __fastcall C_00872abc_FindEntryIndexByName(
    C_00872abc* self, void* /*edx*/, char* name)
{
    // The list object lives behind self->field_0004; its +4 / +8 hold the element range.
    auto* list = reinterpret_cast<std::uint8_t*>(self->field_0004);
    const std::uint32_t beginAddress = CSF_AT(std::uint32_t, list, 0x04);

    std::uint32_t count = 0;
    if (beginAddress != 0)
    {
        // Element size is 164 bytes; signed division by 164 via multiply-high and shift.
        const std::int32_t byteCount = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, list, 0x08) - beginAddress);
        const std::int64_t product = 0x63E7063FLL * byteCount;
        const std::int32_t highProduct = static_cast<std::int32_t>(product >> 32);
        const std::int32_t scaledCount = highProduct >> 6;
        count = static_cast<std::uint32_t>(
            scaledCount + static_cast<std::int32_t>(
                static_cast<std::uint32_t>(scaledCount) >> 31));
    }

    if (count == 0)
        return -1;

    std::uint32_t offset = 0;
    for (std::uint32_t index = 0; index < count; ++index, offset += 164)
    {
        // The original reloads the list pointer on every iteration.
        list = reinterpret_cast<std::uint8_t*>(self->field_0004);
        auto* entry = CSF_AT(std::uint8_t*, list, 0x04) + offset;

        const std::uint32_t capacity = CSF_AT(std::uint32_t, entry, 0xA0);
        const char* entryName = capacity < 0x10
            ? reinterpret_cast<const char*>(entry + 0x8C)
            : CSF_AT(const char*, entry, 0x8C);

        if (entryName && name && __stricmp(entryName, name) == 0)
            return static_cast<int>(index);
    }

    return -1;
}

HOOK(0x0078BBC0, C_00872abc_FindEntryIndexByName, "auto");
