#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue_8BB738, 0x006E04A0, ());
CSF_FUNC(int, __fastcall, ParticleEmitters_LookupParticleSlot, 0x006E10F0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

// Sends the emitter's larger scale value, plus the object's parameters, to the
// global object's virtual method at slot 27 (byte offset 0x6C).
static char __fastcall C_00862598_SendParticleSlotParams(C_00862598* self, void* /*edx*/)
{
    if (!self->field_0048)
        return 0;

    // Read before any call, as the original does.
    uint32_t slotIndex = self->field_0024;

    // The result of GetValue_8BB738 is the `this` for the lookup call.
    int emitterBase = GetValue_8BB738();
    int slot = ParticleEmitters_LookupParticleSlot(reinterpret_cast<void*>(static_cast<uintptr_t>(emitterBase)),
                                                   nullptr, static_cast<int>(slotIndex));

    float valueA = CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(slot)), 0x30);
    float valueB = CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(slot)), 0x34);
    float larger = (valueA >= valueB) ? valueA : valueB;

    // Parameter block: { uint32 field_0030, float field_0034, float field_0038, float larger }
    uint32_t params[4];
    params[0] = self->field_0030;
    *reinterpret_cast<float*>(&params[1]) = self->field_0034;
    *reinterpret_cast<float*>(&params[2]) = CSF_AT(float, self, 0x38);
    *reinterpret_cast<float*>(&params[3]) = larger;

    int globalValueAddr = GetGlobalValue_6C5760();
    void* globalValue = reinterpret_cast<void*>(static_cast<uintptr_t>(globalValueAddr));
    void* gameObj = CSF_AT(void*, globalValue, 0xF0);

    void* targetRegion = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x4C);
    return reinterpret_cast<char(__fastcall*)(void*, void*, uint32_t*, void*, int)>(
        CSF_AT(void**, gameObj, 0)[0x6C / 4])(gameObj, nullptr, params, targetRegion, 0);
}

HOOK(0x006F2950, C_00862598_SendParticleSlotParams, "auto");
