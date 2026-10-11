#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint32_t __fastcall C_00840f48_ProcessParticleList(
    C_00840f48* self, void* /*edx*/, uint32_t* particles)
{
    uint32_t result = static_cast<uint32_t>(self->vSlot14());
    if (static_cast<uint8_t>(result))
    {
        result = particles[2];
        for (uint32_t index = 0; index < result; ++index)
        {
            auto entries = reinterpret_cast<uint32_t**>(particles[0]);
            reinterpret_cast<void (__fastcall*)(C_00840f48*, void*, uint32_t*)>(
                self->vtbl[0])(self, nullptr, entries[index]);
            result = particles[2];
        }
    }
    return result;
}

HOOK(0x0051E5B0, C_00840f48_ProcessParticleList, "auto");
