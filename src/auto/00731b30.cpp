#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: 0x007319C0 CanConfigureEffect(this, effect) -> object pointer or null.
CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0, (void* self, void* edx, int effect));

// Returns the 4-dword configuration vector for effect 2 into `outVector`.
// Uses the effect object's virtual slot 5 (+0x14) when present, otherwise the default (0, 0, 0, 1.0f).
static uint32_t* __fastcall C_00867078_GetConfiguredEffectVector(C_00867078* self, void* /*edx*/, uint32_t* outVector)
{
    uint32_t effectVectorStorage[4];
    uint32_t* sourceVector;

    void* effectObject = reinterpret_cast<void*>(CanConfigureEffect(self, nullptr, 2));
    if (effectObject)
    {
        // Virtual slot +0x14 of the effect object: fills effectVectorStorage and returns a pointer to the data.
        sourceVector = ((uint32_t* (__fastcall*)(void*, void*, uint32_t*))
                        CSF_AT(void**, effectObject, 0)[5])(effectObject, nullptr, effectVectorStorage);
    }
    else
    {
        std::memset(effectVectorStorage, 0, 12);
        effectVectorStorage[3] = 0x3F800000u; // 1.0f
        sourceVector = effectVectorStorage;
    }

    outVector[0] = sourceVector[0];
    outVector[1] = sourceVector[1];
    outVector[2] = sourceVector[2];
    outVector[3] = sourceVector[3];
    return outVector;
}

HOOK(0x00731B30, C_00867078_GetConfiguredEffectVector, "auto");
