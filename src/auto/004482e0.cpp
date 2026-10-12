#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static std::uint64_t __fastcall S_004480f0_AdvanceTime(
    S_004480f0* self, void* /*edx*/, float elapsed)
{
    if (!self->field_0028)
    {
        const std::uint32_t count = self->field_0034;
        if (count)
        {
            const std::uint32_t remaining = count - 1;
            if (remaining)
            {
                const std::uint32_t entries = self->field_002c;
                self->field_0034 = remaining;
                _memcpy(reinterpret_cast<void*>(entries),
                        reinterpret_cast<const void*>(entries + 12),
                        12 * remaining);
            }
            else
            {
                self->field_0034 = 0;
            }
        }
    }

    self->field_0028 = 0;
    const std::int64_t result = static_cast<std::int64_t>(elapsed);
    self->field_0024 = static_cast<std::uint32_t>(result);
    if (!static_cast<std::uint32_t>(result))
        self->field_0024 = 1;

    self->field_005c = self->field_005c - elapsed;
    self->field_0060 = self->field_0060 - elapsed;
    return static_cast<std::uint64_t>(result);
}

HOOK(0x004482E0, S_004480f0_AdvanceTime, "auto");
