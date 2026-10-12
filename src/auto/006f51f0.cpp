#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, Particle_ApplyPositionDelta, 0x006F44C0, (void* self, void* edx, float* a2));

// ecx points 0x20 bytes past the particle object (embedded secondary base).
static void __fastcall RpPrtStd_UpdateAtomicPosition(void* self, void* /*edx*/, float** a2, char a3)
{
    uint8_t* particleBase = reinterpret_cast<uint8_t*>(self) - 0x20;
    void* particle = particleBase;

    // Slot 0x48 (18): returns pointer to the current position vector.
    float* currentPos = ((float* (__fastcall*)(void*, void*))CSF_AT(void**, particle, 0)[18])(particle, nullptr);
    float originX = currentPos[0];
    float originY = currentPos[1];
    float originZ = currentPos[2];

    // Slot 0x40 (16): virtual update taking float** as its only argument.
    ((void (__fastcall*)(void*, void*, float**))CSF_AT(void**, particle, 0)[16])(particle, nullptr, a2);

    if (a3 != 0)
    {
        float* newPos = *a2;
        if (originX != newPos[12] || originY != newPos[13] || originZ != newPos[14])
        {
            float delta[3];
            delta[0] = newPos[12] - originX;
            delta[1] = newPos[13] - originY;
            delta[2] = newPos[14] - originZ;
            Particle_ApplyPositionDelta(particle, nullptr, delta);
        }
    }
}

HOOK(0x006F51F0, RpPrtStd_UpdateAtomicPosition, "auto");
