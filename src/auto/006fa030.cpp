#include <cstdint>
#include <cstring>
#include "hook.h"

static void __stdcall GfxScreenEffects_CopyThirtyTwoByteElement(
    void* destination, const void* source)
{
    if (!destination)
        return;

    auto* output = static_cast<std::uint8_t*>(destination);
    const auto* input = static_cast<const std::uint8_t*>(source);
    for (std::uint32_t offset = 0; offset < 0x20u; offset += 4)
    {
        std::uint32_t word;
        std::memcpy(&word, input + offset, sizeof(word));
        std::memcpy(output + offset, &word, sizeof(word));
    }
}

HOOK(0x006FA030, GfxScreenEffects_CopyThirtyTwoByteElement, "auto");
