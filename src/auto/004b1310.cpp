#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static int __fastcall AiFncMacroOrder_PopTargetPosition(
    void* self, void* /*edx*/, std::uint32_t* position)
{
    position[0] = CSF_AT(std::uint32_t*, self, 0x10)[0];
    std::uint32_t remaining = CSF_AT(std::uint32_t, self, 0x18) - 1;
    if (remaining == 0)
    {
        CSF_AT(std::uint32_t, self, 0x18) = 0;
    }
    else
    {
        auto* data = CSF_AT(std::uint32_t*, self, 0x10);
        CSF_AT(std::uint32_t, self, 0x18) = remaining;
        _memcpy(data, data + 1, remaining * 4);
    }

    position[1] = CSF_AT(std::uint32_t*, self, 0x10)[0];
    remaining = CSF_AT(std::uint32_t, self, 0x18) - 1;
    if (remaining == 0)
    {
        CSF_AT(std::uint32_t, self, 0x18) = 0;
    }
    else
    {
        auto* data = CSF_AT(std::uint32_t*, self, 0x10);
        CSF_AT(std::uint32_t, self, 0x18) = remaining;
        _memcpy(data, data + 1, remaining * 4);
    }

    position[2] = CSF_AT(std::uint32_t*, self, 0x10)[0];
    remaining = CSF_AT(std::uint32_t, self, 0x18) - 1;
    if (remaining == 0)
    {
        CSF_AT(std::uint32_t, self, 0x18) = 0;
        return 0;
    }

    CSF_AT(std::uint32_t, self, 0x18) = remaining;
    auto* data = CSF_AT(std::uint32_t*, self, 0x10);
    return static_cast<int>(reinterpret_cast<std::intptr_t>(
        _memcpy(data, data + 1, remaining * 4)));
}

HOOK(0x004B1310, AiFncMacroOrder_PopTargetPosition, "auto");
