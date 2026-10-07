#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall FindActiveEntryIndexByKey(
    void* self, void* /*edx*/, void* keyObject)
{
    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x08);
    if (count == 0)
        return -1;

    auto* entry = CSF_AT(std::uint8_t*, self, 0x00);
    std::uint32_t index = 0;

    do
    {
        if (CSF_AT(std::uint32_t, entry, 0x00) != 0 &&
            CSF_AT(std::uint32_t, entry, 0x08) ==
                CSF_AT(std::uint32_t, keyObject, 0x04))
        {
            return static_cast<int>(index);
        }

        ++index;
        entry += 0x1C;
    } while (index < count);

    return -1;
}

HOOK(0x004DC5B0, FindActiveEntryIndexByKey, "auto");
