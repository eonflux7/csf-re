#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static int __fastcall Ai_ReadDebugTriple(
    void* self, void* /*edx*/,
    std::uint32_t* first, std::uint32_t* second, std::uint32_t* third)
{
    auto* front = CSF_AT(std::uint32_t*, self, 0x10);
    *first = *front;
    std::uint32_t remaining = CSF_AT(std::uint32_t, self, 0x18) - 1;
    if (remaining == 0)
    {
        CSF_AT(std::uint32_t, self, 0x18) = 0;
    }
    else
    {
        auto* data = CSF_AT(std::uint8_t*, self, 0x10);
        CSF_AT(std::uint32_t, self, 0x18) = remaining;
        _memcpy(data, data + 4, 4 * remaining);
    }

    front = CSF_AT(std::uint32_t*, self, 0x10);
    *second = *front;
    remaining = CSF_AT(std::uint32_t, self, 0x18) - 1;
    if (remaining == 0)
    {
        CSF_AT(std::uint32_t, self, 0x18) = 0;
    }
    else
    {
        auto* data = CSF_AT(std::uint8_t*, self, 0x10);
        CSF_AT(std::uint32_t, self, 0x18) = remaining;
        _memcpy(data, data + 4, 4 * remaining);
    }

    front = CSF_AT(std::uint32_t*, self, 0x10);
    *third = *front;
    remaining = CSF_AT(std::uint32_t, self, 0x18) - 1;
    if (remaining == 0)
    {
        CSF_AT(std::uint32_t, self, 0x18) = 0;
        return 0;
    }

    CSF_AT(std::uint32_t, self, 0x18) = remaining;
    auto* data = CSF_AT(std::uint8_t*, self, 0x10);
    return static_cast<int>(
        reinterpret_cast<std::uintptr_t>(_memcpy(data, data + 4, 4 * remaining)));
}

HOOK(0x004B70B0, Ai_ReadDebugTriple, "auto");
