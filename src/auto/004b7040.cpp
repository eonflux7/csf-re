#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static int __fastcall ActionArgs_ReadPair(void* self, void* /*edx*/,
                                         std::uint32_t* first,
                                         std::uint32_t* second)
{
    auto* data = CSF_AT(std::uint32_t*, self, 0x10);
    *first = *data;
    std::uint32_t remaining = CSF_AT(std::uint32_t, self, 0x18) - 1;
    if (remaining == 0)
    {
        CSF_AT(std::uint32_t, self, 0x18) = 0;
    }
    else
    {
        data = CSF_AT(std::uint32_t*, self, 0x10);
        CSF_AT(std::uint32_t, self, 0x18) = remaining;
        _memcpy(data, data + 1, remaining * 4u);
    }

    data = CSF_AT(std::uint32_t*, self, 0x10);
    *second = *data;
    remaining = CSF_AT(std::uint32_t, self, 0x18) - 1;
    if (remaining == 0)
    {
        CSF_AT(std::uint32_t, self, 0x18) = 0;
        return 0;
    }

    CSF_AT(std::uint32_t, self, 0x18) = remaining;
    data = CSF_AT(std::uint32_t*, self, 0x10);
    return reinterpret_cast<int>(_memcpy(data, data + 1, remaining * 4u));
}

HOOK(0x004B7040, ActionArgs_ReadPair, "auto");
