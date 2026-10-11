#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static void __fastcall ParticleManager_RemoveEffectById(void* self, void* /*edx*/, int effectId)
{
    if (effectId == -1)
        return;

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x4C);
    if (count == 0)
        return;

    auto* entry = CSF_AT(std::uint8_t*, self, 0x44);
    std::uint32_t index = 0;
    while (CSF_AT(int, entry, 0x0C) != effectId)
    {
        ++index;
        entry += 16;
        if (index >= count)
            return;
    }

    void* effect = CSF_AT(void*, entry, 0);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, effect, 0)[0x2C / 4])(effect, nullptr, 1);

    const std::uint32_t remaining = CSF_AT(std::uint32_t, self, 0x4C) - 1;
    CSF_AT(std::uint32_t, self, 0x4C) = remaining;
    if (index != remaining)
    {
        auto* destination = CSF_AT(std::uint8_t*, self, 0x44) + 16 * index;
        _memcpy(destination, destination + 16, 16 * (remaining - index));
    }
}

HOOK(0x0051FB00, ParticleManager_RemoveEffectById, "auto");
