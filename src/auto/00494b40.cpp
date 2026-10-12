#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound original callee: GetObjectPositionVector (0x48E430). The caller passes only ecx,
// with no pushes, so it is __fastcall(self, edx) and returns a pointer to a 3-dword vector.
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x48E430, (void* self, void* edx));

// Fills outPosition (a 3-dword position vector, float bit patterns) for the player character.
// If the character's flag at +0x228 is set, it delegates to virtual slot 0xA4 (slot 41)
// and returns the output buffer unchanged. Otherwise it copies either the override vector
// at +0x410 or the object position vector plus the Y offset at +0x1D4.
static float* __fastcall CPlayerCharacter_FillPositionVector(CPlayerCharacter* self, void* /*edx*/, float* outPosition)
{
    if (self->field_0228)
    {
        // Virtual slot 0xA4 / 4 = 41; called through the vtable because no typed wrapper is bound here.
        ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, self, 0)[41])(self, nullptr, outPosition);
        return outPosition;
    }

    uint32_t vectorX;
    uint32_t vectorY;
    uint32_t vectorZ;

    if (self->field_0404)
    {
        vectorX = CSF_AT(uint32_t, self, 0x410);
        vectorY = CSF_AT(uint32_t, self, 0x414);
        vectorZ = CSF_AT(uint32_t, self, 0x418);
    }
    else
    {
        uint32_t* objectPositionVector = GetObjectPositionVector(self, nullptr);
        vectorX = objectPositionVector[0];

        float adjustedY = *reinterpret_cast<float*>(&objectPositionVector[1]) + CSF_AT(float, self, 0x1D4);
        std::memcpy(&vectorY, &adjustedY, sizeof(vectorY));

        vectorZ = objectPositionVector[2];
    }

    uint32_t* outBits = reinterpret_cast<uint32_t*>(outPosition);
    outBits[0] = vectorX;
    outBits[1] = vectorY;
    outBits[2] = vectorZ;
    return outPosition;
}

HOOK(0x494B40, CPlayerCharacter_FillPositionVector, "auto");
