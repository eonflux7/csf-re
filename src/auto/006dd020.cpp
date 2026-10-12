#include <cstdint>
#include <cstring>
#include "hook.h"

static void __stdcall TextureLoader_CopySevenDwordElement(
    void* destination, const void* source)
{
    if (!destination)
        return;

    auto* output = static_cast<std::uint8_t*>(destination);
    const auto* input = static_cast<const std::uint8_t*>(source);
    for (std::uint32_t offset = 0; offset < 0x1Cu; offset += 4)
    {
        std::uint32_t word;
        std::memcpy(&word, input + offset, sizeof(word));
        std::memcpy(output + offset, &word, sizeof(word));
    }
}

HOOK(0x006DD020, TextureLoader_CopySevenDwordElement, "auto");
