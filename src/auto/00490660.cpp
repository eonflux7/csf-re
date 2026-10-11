#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(int, __fastcall, GrowEntryArray, 0x0048EAC0, (void* self, void* edx, int a2));

static int __fastcall AppendEntry(void* self, void* /*edx*/, const std::uint32_t* entry)
{
    auto* dirtyFlag = CSF_AT(std::uint8_t*, self, 0xA8);
    CSF_AT(std::uint8_t, self, 0x171) = 1;
    auto* array = static_cast<std::uint8_t*>(self) + 0xBC;
    *dirtyFlag = 1;

    if (CSF_AT(std::uint32_t, self, 0xC4) >=
        CSF_AT(std::uint32_t, self, 0xC0))
    {
        GrowEntryArray(array, nullptr, CSF_AT(int, self, 0xC8));
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0xC4);
    auto* data = CSF_AT(std::uint8_t*, self, 0xBC);
    CSF_AT(std::uint32_t, self, 0xC4) = count + 1;
    auto* destination = data + 12 * count;
    CSF_AT(std::uint32_t, destination, 0) = entry[0];
    CSF_AT(std::uint32_t, destination, 4) = entry[1];
    const std::uint32_t result = entry[2];
    CSF_AT(std::uint32_t, destination, 8) = result;
    return static_cast<int>(result);
}

HOOK(0x00490660, AppendEntry, "auto");
