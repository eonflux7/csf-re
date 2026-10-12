#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static int __fastcall RemoveFirstQueuedEntry(void* self, void* /*edx*/,
                                            int /*unused*/, void* output)
{
    auto* entries = CSF_AT(std::uint8_t*, self, 0x84);
    CSF_AT(std::uint32_t, output, 4) = CSF_AT(std::uint32_t, entries, 4);
    CSF_AT(std::uint32_t, output, 8) = CSF_AT(std::uint32_t, entries, 8);

    const std::uint32_t remaining = CSF_AT(std::uint32_t, self, 0x8C) - 1;
    if (remaining == 0)
    {
        CSF_AT(std::uint32_t, self, 0x8C) = 0;
        return 0;
    }

    CSF_AT(std::uint32_t, self, 0x8C) = remaining;
    entries = CSF_AT(std::uint8_t*, self, 0x84);
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(
        _memcpy(entries, entries + 12, 12 * remaining)));
}

HOOK(0x004B71D0, RemoveFirstQueuedEntry, "auto");
