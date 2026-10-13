#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, Ptank_UpdateGroups, 0x006D7930, (void* self, void* , int updateArgument));

static uint32_t* __fastcall S_006d6800_UpdateObjectsAndParticleGroups(
    S_006d6800* self, void* /*edx*/, int updateArgument)
{
    for (uint32_t index = 0; index < CSF_AT(uint32_t, self, 0x1C); ++index)
    {
        void* object = CSF_AT(void**, self, 0x14)[index];
        if (object)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0x30 / 4])(
                    object, nullptr, updateArgument);
        }
    }

    uint32_t remainingGroups = self->field_003c;
    uint32_t groupOffset = 0;
    while (remainingGroups != 0)
    {
        void* descriptor = reinterpret_cast<void*>(
            CSF_AT(uint32_t, self, 0x34) + groupOffset);
        const uint32_t objectBase = CSF_AT(uint32_t, descriptor, 0);
        const uint32_t objectStride = CSF_AT(uint32_t, descriptor, 4);
        const uint32_t totalBytes =
            objectStride * CSF_AT(uint32_t, descriptor, 8);

        uint32_t objectOffset = 0;
        while (objectOffset < totalBytes)
        {
            void* object = reinterpret_cast<void*>(objectBase + objectOffset);
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0x30 / 4])(
                    object, nullptr, updateArgument);
            objectOffset += objectStride;
        }

        groupOffset += 12;
        --remainingGroups;
    }

    uint32_t* particleGroups = CSF_AT(uint32_t*, self, 0x48);
    if (particleGroups)
    {
        return reinterpret_cast<uint32_t*>(
            Ptank_UpdateGroups(particleGroups, nullptr, updateArgument));
    }
    return particleGroups;
}

HOOK(0x006DFE00, S_006d6800_UpdateObjectsAndParticleGroups, "auto");
