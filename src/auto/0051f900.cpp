#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static unsigned int __fastcall FxParticleManager_ApplyHandleValueChange(
    void* self, void* /*edx*/, void* handle, int value)
{
    unsigned int count = CSF_AT(unsigned int, self, 0x64);
    unsigned int index = 0;
    unsigned int offset = 0;

    while (index < count)
    {
        auto* entry = CSF_AT(std::uint8_t*, self, 0x5C) + offset;
        if (CSF_AT(std::uint32_t, entry, 0x08) ==
                CSF_AT(std::uint32_t, handle, 0x04) &&
            CSF_AT(int, entry, 0x10) == value)
        {
            void* particle = CSF_AT(void*, entry, 0x14);
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, particle, 0)[0x2C / 4])(
                    particle, nullptr, 0);
        }

        count = CSF_AT(unsigned int, self, 0x64);
        ++index;
        offset += 0x24;
    }

    return count;
}

HOOK(0x0051F900, FxParticleManager_ApplyHandleValueChange, "auto");
